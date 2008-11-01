/***************************************************************************
 *   Copyright (C) 2008 by Lorenzo Masini                                  *
 *   lorenxo86@gmail.com                                                   *
 *   Copyright (C) 2008 by Andrea Decorte                                  *
 *   adecorte@gmail.com                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <QVBoxLayout>
#include <QPen>
#include <QColor>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QTextStream>
#include <QIODevice>
#include <QDir>
#include <QStringList>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomText>

#include "centralwidget.h"
#include "osmmapadapter.h"
#include "maplayer.h"
#include "geometrylayer.h"
#include "geometry.h"
#include "circlepoint.h"

#include "networkdialog.h"

CentralWidget::CentralWidget( QWidget *parent ) 
	: QWidget(parent)
{
	QSettings settings;

	control = new MapControl(QSize(600, 600));
	connect(control, SIGNAL(viewChanged(const QPointF &, int)), this, SIGNAL(viewChanged(const QPointF &, int)));	
	
	adapter = new OSMMapAdapter();
	base = new MapLayer("base", adapter);
	
	control->addLayer(base);
	
	layers[None] = new GeometryLayer("None", adapter);
	layers[WEP] = new GeometryLayer("WEP", adapter);
	layers[WPA] = new GeometryLayer("WPA", adapter);
	layers[WPA2] = new GeometryLayer("WPA2", adapter);
	
	for(int i = 0; i<=3; i++)
		control->addLayer(layers[i]);
	
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(control);
	layout->setMargin(0);	
	setLayout(layout);


	/* Cache */
	if(settings.value("settings/cache_enabled", false).toBool())
		control->enablePersistentCache(settings.value("settings/cache_dir", QDir::homePath() + "/QMapControl.cache").toString());
	
	/* Proxy */
	if(settings.value("settings/proxy_enabled", false).toBool())
		control->setProxy(settings.value("settings/proxy_address", QString()).toString(), settings.value("settings/proxy_port", 3829).toInt());

	/* View */
	control->setView(QPointF(settings.value("settings/view_lon", 0.0).toDouble(), settings.value("settings/view_lat", 0.0).toDouble()));
	control->setZoom(settings.value("settings/view_zoom", 2).toInt());
}

void CentralWidget::loadNetworks()
{
	QSqlQuery query;
	if(!query.exec("SELECT bssid, lat, lon, encryption FROM networks"))
		return;

	while(query.next()){
		QString bssid = query.value(0).toString();
		qreal lat = query.value(1).toDouble();		
		qreal lon = query.value(2).toDouble();
		int encryption = query.value(3).toInt();
		addNetwork((NetworkEncryption)encryption, lon, lat, bssid);
	}
}

void CentralWidget::importNetworksFromCSV(QIODevice &device)
{
	QTextStream stream(&device);
		
	while(!stream.atEnd()){	
		QString line = stream.readLine();
		importNetwork(line);
		}
		
	clearNetworks();
	loadNetworks();	
}

void CentralWidget::importNetworksFromXML(QIODevice &device) 
{
	QDomDocument doc("XML import");
	QString error;
	if (!doc.setContent(&device, &error)) {
		qDebug("Error while opening");
		return;
	}

 // print out the element names of all elements that are direct children
 // of the outermost element.
 	QDomElement root = doc.documentElement();
    if (root.tagName() != "detection-run") {
       	QMessageBox::warning(this, "Error", tr("Invalid XML file"));
    	return;
   	}
   	
 	QDomNodeList networks = root.childNodes(); //node wireless-network

	//first cycle on networks
	for (unsigned int i= 0; i < networks.length(); ++i) {
		importNetwork(networks.item(i));			
	}
	
	clearNetworks();
	loadNetworks();
}


void CentralWidget::addNetwork()
{
	QPointF coordinate = control->currentCoordinate();
	NetworkDialog dialog(NetworkDialog::CreateMode, this);
	
	dialog.setLatitude(coordinate.y());
	dialog.setLongitude(coordinate.x());

	if(dialog.exec()){
		QSqlQuery writeQuery;
		writeQuery.prepare("INSERT INTO networks (essid,bssid,channel,signal,lat,lon,comment,encryption) VALUES (:essid,:bssid,:channel,:signal,:latitude,:longitude,:comment,:encryption)");	
		writeQuery.bindValue(":bssid", dialog.bssid());
		writeQuery.bindValue(":essid", dialog.essid());
		writeQuery.bindValue(":channel", dialog.channel());
		writeQuery.bindValue(":signal", dialog.signal());
		writeQuery.bindValue(":latitude", dialog.latitude());
		writeQuery.bindValue(":longitude", dialog.longitude());
		writeQuery.bindValue(":comment", dialog.comment());
		writeQuery.bindValue(":encryption", dialog.encryption());
		if(!writeQuery.exec()){
			QMessageBox::warning(this, writeQuery.lastError().driverText(), writeQuery.lastError().databaseText());			
			return;
		}
		clearNetworks();
		loadNetworks();
	}
}

void CentralWidget::clearNetworks()
{
	for(int i = 0; i <= 3; i++)
		layers[i]->clearGeometries();
	control->updateRequestNew();
}

void CentralWidget::goHome()
{
	QSettings settings;
	
	control->moveTo(QPointF(settings.value("settings/view_lon", 0.0).toDouble(), settings.value("settings/view_lat", 0.0).toDouble()));
	control->setZoom(settings.value("settings/view_zoom", 2).toInt());
}

void CentralWidget::showNetwork(Geometry *geometry, QPoint /*point*/)
{
	QSqlQuery query;
	query.prepare("SELECT * from networks WHERE bssid=:bssid");
	query.bindValue(":bssid", geometry->name());
	if(!query.exec()){
		QMessageBox::warning(this, query.lastError().driverText(), query.lastError().databaseText());			
		return;
	}

	if(!query.next())
		return;
		
	NetworkDialog dialog(NetworkDialog::ModifyMode, this);
	dialog.setEssid(query.value(0).toString());
	dialog.setBssid(query.value(1).toString());
	dialog.setChannel(query.value(2).toInt());
	dialog.setSignal(query.value(3).toInt());
	dialog.setLatitude(query.value(4).toDouble());
	dialog.setLongitude(query.value(5).toDouble());
	dialog.setComment(query.value(6).toString());
	dialog.setEncryption(query.value(7).toInt());
	
	if(dialog.exec()){
		QSqlQuery writeQuery;
		NetworkDialog::Purpose purpose = dialog.purpose();
		switch(purpose){
			case NetworkDialog::SaveNetworkAction:
				writeQuery.prepare("UPDATE networks SET essid=:essid, bssid=:bssid, channel=:channel, signal=:signal, lat=:latitude, lon=:longitude, comment=:comment, encryption=:encryption WHERE bssid=:bssidprev");	
				writeQuery.bindValue(":bssid", dialog.bssid());
				writeQuery.bindValue(":essid", dialog.essid());
				writeQuery.bindValue(":channel", dialog.channel());
				writeQuery.bindValue(":signal", dialog.signal());
				writeQuery.bindValue(":latitude", dialog.latitude());
				writeQuery.bindValue(":longitude", dialog.longitude());
				writeQuery.bindValue(":comment", dialog.comment());
				writeQuery.bindValue(":encryption", dialog.encryption());
				writeQuery.bindValue(":bssidprev", geometry->name());
				break;
			case NetworkDialog::RemoveNetworkAction:
				writeQuery.prepare("DELETE FROM networks WHERE bssid=:bssid");
				writeQuery.bindValue(":bssid", query.value(1).toString());
				break;
				
			case NetworkDialog::NoAction:
				return;
		}
		if(!writeQuery.exec()){
			QMessageBox::warning(this, writeQuery.lastError().driverText(), writeQuery.lastError().databaseText());			
			return;
		}
		clearNetworks();
		loadNetworks();
	}
}

void CentralWidget::importNetwork(const QString &line)
{
	QStringList fields = line.split(";");
	NetworkEncryption encryption = None;

	if(fields.at(1) != "infrastructure")
		return;
		
	if(fields.at(2) == "<no ssid>")
		return;
	
	if(fields.at(7).contains("None"))
		encryption = None;
	
	if(fields.at(7).contains("WEP"))
		encryption = WEP;
		
	if(fields.at(7).contains("TKIP"))
		encryption = WPA;
		
	if(fields.at(7).contains("AES-CCM"))
		encryption = WPA2;
			
	QSqlQuery query;
	query.prepare("INSERT INTO networks (essid,bssid,channel,signal,lat,lon,comment,encryption) VALUES (:essid,:bssid,:channel,:signal,:lat,:lon,NULL,:encryption)");
	query.bindValue(":essid", fields.at(2));
	query.bindValue(":bssid", fields.at(3));
	query.bindValue(":channel", fields.at(5));
	query.bindValue(":signal", fields.at(22));
	query.bindValue(":lat", fields.at(32));
	query.bindValue(":lon", fields.at(33));
	query.bindValue(":encryption", encryption);
	if(!query.exec()){
		if(query.lastError().number() != 19){ // during import it's possible to have duplicated entries. This silently don't insert the duplicated rows. 
			QMessageBox::warning(this, query.lastError().driverText(), query.lastError().databaseText());
			return;
		}
	}
}

void CentralWidget::importNetwork(const QDomNode &node)
{
	QString SSID;
    QString BSSID;
    QString channel;
    //QString maxrate;
    QString longitude;
    QString latitude;
    NetworkEncryption encryption = None;
	QDomNodeList elements = node.childNodes();

	for (unsigned int i = 0; i < elements.length(); ++i) {
		QDomNode element = elements.item(i);
	 	QDomElement e = element.toElement(); // try to convert the node to an element.
     	if(!e.isNull()) {
			if (e.tagName() == "SSID") {
				SSID = e.text();
			}
			if (e.tagName() == "BSSID") {
				BSSID = e.text();
			}
			if (e.tagName() == "channel") {
				channel = e.text();
			}
			//if (e.tagName() == "maxrate") {
			//	maxrate = e.text();
			//}
			if (e.tagName() == "gps-info") {
				QDomNodeList locations = e.childNodes();
				for (unsigned int i = 0; i < locations.length(); ++i) {
					QDomNode location = locations.item(i);
					QDomElement l = location.toElement(); // try to convert the node to an element.
					//we could also calculate the media among min lat and max lat									
					if (l.tagName() == "min-lat") {
						latitude = l.text();
					}
					if (l.tagName() == "min-lon") {
						longitude = l.text();
					}
				}
			}
			//the last encryption is the strongest
			if (e.tagName() == "encryption") {
				if (e.text() == "None")
					encryption = None;
				if (e.text() == "WEP")
					encryption = WEP;
				if (e.text() == "WPA" || e.text() == "TKIP")
					encryption = WPA;
				if (e.text() == "AES-CCM")
					encryption = WPA2;
				}				
     		}
    	}
     			
     	QSqlQuery query;
		query.prepare("INSERT INTO networks (essid,bssid,channel,signal,lat,lon,comment,encryption) VALUES (:essid,:bssid,:channel,:signal,:lat,:lon,NULL,:encryption)");
		query.bindValue(":essid",SSID);
		//qDebug(SSID.toUtf8());
		query.bindValue(":bssid", BSSID);
		query.bindValue(":channel", channel);
		query.bindValue(":signal", 0); //we can't get this value from XML file
		query.bindValue(":lat", latitude);
		query.bindValue(":lon", longitude);
		query.bindValue(":encryption", encryption);
		if(!query.exec()){
			if(query.lastError().number() != 19){ // during import it's possible to have duplicated entries. This silently don't insert the duplicated rows. 
				QMessageBox::warning(this, query.lastError().driverText(), query.lastError().databaseText());
				return;
			}
		}
}

void CentralWidget::resizeEvent(QResizeEvent *event)
{
	control->resize(event->size() - QSize (1,1));	
}

void CentralWidget::addNetwork(NetworkEncryption encryption, qreal x, qreal y, QString name)
{
	QPen pen;
	switch(encryption){
		case None:
			pen.setColor(QColor(Qt::red));
			break;
		case WEP:
			pen.setColor(QColor(Qt::yellow));
			break;
		case WPA:
			pen.setColor(QColor(Qt::blue));
			break;
		case WPA2:
			pen.setColor(QColor(Qt::green));	
	}			
	pen.setWidth(3);
		
	CirclePoint *network = new CirclePoint(x, y, 15, name, qmapcontrol::Point::Middle, &pen);
	connect(network, SIGNAL(geometryClicked(Geometry *, QPoint)), this, SLOT(showNetwork(Geometry *, QPoint)));	
	layers[encryption]->addGeometry(network);
}

