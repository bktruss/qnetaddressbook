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
#include "viewpreferencespage.h"

ViewPreferencesPage::ViewPreferencesPage( QWidget *parent ) 
	: PreferencesPage(parent), Ui::ViewPreferencesPage()
{
	setupUi(this);
	
	lonSpin->setValue(settings.value("settings/view_lon", 0.0).toDouble());
	latSpin->setValue(settings.value("settings/view_lat", 0.0).toDouble());
	zoomSpin->setValue(settings.value("settings/view_zoom", 2).toInt());
	
	connect(lonSpin, SIGNAL(valueChanged(double)), this, SIGNAL(settingsChanged()));
	connect(latSpin, SIGNAL(valueChanged(double)), this, SIGNAL(settingsChanged()));
	connect(zoomSpin, SIGNAL(valueChanged(int)), this, SIGNAL(settingsChanged()));
}

void ViewPreferencesPage::applyChanges()
{
	settings.setValue("settings/view_lon", lonSpin->value());
	settings.setValue("settings/view_lat", latSpin->value());
	settings.setValue("settings/view_zoom", zoomSpin->value());		
}

void ViewPreferencesPage::restoreDefaults()
{
	settings.setValue("settings/view_lon", 0.0);
	settings.setValue("settings/view_lat", 0.0);
	settings.setValue("settings/view_zoom", 2);		
	
	lonSpin->setValue(settings.value("settings/view_lon", 0.0).toDouble());
	latSpin->setValue(settings.value("settings/view_lat", 0.0).toDouble());
	zoomSpin->setValue(settings.value("settings/view_zoom", 2).toInt());
}