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
    if(!query.exec("SELECT bssid, lat, lon, encryption FROM networks")){
        QMessageBox::critical(this, query.lastError().driverText(), query.lastError().databaseText());
        return;
    }
    while(query.next()){
        QString bssid = query.value(0).toString();
        qreal lat = query.value(1).toDouble();
        qreal lon = query.value(2).toDouble();
        int encryption = query.value(3).toInt();
        addNetwork((NetworkEncryption)encryption, lon, lat, bssid);
    }
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

void CentralWidget::importNetworks(QList<Network> networks)
{
    foreach(Network network, networks){
        QSqlQuery query;
        query.prepare("INSERT INTO networks (essid,bssid,channel,signal,lat,lon,comment,encryption) VALUES (:essid,:bssid,:channel,:signal,:lat,:lon,:comment,:encryption)");
        query.bindValue(":essid", network.essid);
        query.bindValue(":bssid", network.bssid);
        query.bindValue(":channel", network.channel);
        query.bindValue(":signal", network.signal);
        query.bindValue(":lat", network.lat);
        query.bindValue(":lon", network.lon);
        query.bindValue(":encryption", network.encryption);
        if(!query.exec()){
            if(query.lastError().number() != 19){ // during import it's possible to have duplicated entries. This silently don't insert the duplicated rows.
                QMessageBox::warning(this, query.lastError().driverText(), query.lastError().databaseText());
                return;
            }
        }
    }
    clearNetworks();
    loadNetworks();
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

void CentralWidget::moveTo(const QPointF &coordinate)
{
    control->moveTo(coordinate);
    control->setZoom(17);
}

void CentralWidget::resizeEvent(QResizeEvent *event)
{
    control->resize(event->size() - QSize (1,1));
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
