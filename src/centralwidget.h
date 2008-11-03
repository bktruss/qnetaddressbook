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
#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include "mapcontrol.h"
#include "mapadapter.h"
#include "layer.h"

using namespace qmapcontrol;

class QIODevice;
class QResizeEvent;
class QDomNode;
class CentralWidget : public QWidget
{
		Q_OBJECT
	
	public:
		typedef enum {
			None = 0,
			WEP,
			WPA,
			WPA2
		} NetworkEncryption;	
	
		CentralWidget(QWidget *parent = 0);
		/* Load Database */
		void loadNetworks();
		
		/* Import Networks */
		void importNetworksFromCSV(QIODevice &device);
		void importNetworksFromXML(QIODevice &device);
	
	public slots:
		/* Add Network */
		void addNetwork();	
		
		/* Filter Networks */
		void setVisibleOpenNetworks(bool visible){ setVisibleNetworks(None, visible); }
		void setVisibleWEPNetworks(bool visible){ setVisibleNetworks(WEP, visible); }
		void setVisibleWPANetworks(bool visible){ setVisibleNetworks(WPA, visible); }
		void setVisibleWPA2Networks(bool visible){ setVisibleNetworks(WPA2, visible); }	
	
		/* Clear all geometries */
		void clearNetworks();
		
		/* Zoom */
		void zoomIn() {control->zoomIn();}
		void zoomOut() { control->zoomOut();}
		
		/* View */
                void goHome();

                /* Move to */
                void moveTo(const QPointF &coordinate);

	protected:
		virtual void resizeEvent(QResizeEvent *event);	
		
	private slots:
		/* Real methods to do things*/
		void showNetwork(Geometry *geometry, QPoint point);
		void setVisibleNetworks(NetworkEncryption encryption, bool visible) { layers[encryption]->setVisible(visible); }
		void importNetwork(const QString &line);
		void importNetwork(const QDomNode &node);
		
	signals:
		void viewChanged(const QPointF &coordinate, int zoom);
			
	private:
		void addNetwork(NetworkEncryption encryption, qreal x, qreal y, QString name);
		
		MapControl *control;
		MapAdapter *adapter;
		Layer *layers[4];
		Layer *base;	
};
#endif
