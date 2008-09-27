/***************************************************************************
 *   Copyright (C) 2008 by Lorenzo Masini                                  *
 *   lorenxo86@gmail.com                                                   *
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

typedef enum {
	None = 0,
	WEP,
	WPA,
	WPA2
} NetworkEncryption;

class QIODevice;
class CentralWidget : public QWidget
{
		Q_OBJECT
	public:
		CentralWidget(QWidget *parent = 0);
		/* Load Database */
		void loadNetworks();
		
		/* Import Networks */
		void importNetworks(QIODevice &device);
	
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
	
	private slots:
		/* Real methods to do things*/
		void showNetwork(Geometry *geometry, QPoint point);
		void setVisibleNetworks(NetworkEncryption encryption, bool visible) { layers[encryption]->setVisible(visible); }
		void importNetwork(const QString &line);
	
	private:
		void addNetwork(NetworkEncryption encryption, qreal x, qreal y, QString name);
		
		MapControl *control;
		MapAdapter *adapter;
		Layer *layers[4];
		Layer *base;	
};
#endif
