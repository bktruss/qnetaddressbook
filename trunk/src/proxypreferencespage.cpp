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
#include "proxypreferencespage.h"

ProxyPreferencesPage::ProxyPreferencesPage( QWidget *parent) 
    : PreferencesPage(parent), Ui::ProxyPreferencesPage()
{
    setupUi(this);
    groupBox->setChecked(settings.value("settings/proxy_enabled", false).toBool());
    addressEdit->setText(settings.value("settings/proxy_address", QString()).toString());
    portSpin->setValue(settings.value("settings/proxy_port", 3829).toInt());

    connect(addressEdit, SIGNAL(textChanged(const QString &)), this, SIGNAL(settingsChanged()));
    connect(portSpin, SIGNAL(valueChanged(int)), this, SIGNAL(settingsChanged()));
}

void ProxyPreferencesPage::applyChanges()
{
    settings.setValue("settings/proxy_enabled", groupBox->isChecked());
    settings.setValue("settings/proxy_address", addressEdit->text());
    settings.setValue("settings/proxy_port", portSpin->value());
}

void ProxyPreferencesPage::restoreDefaults()
{
    settings.setValue("settings/proxy_enabled", false);
    settings.setValue("settings/proxy_address", QString());
    settings.setValue("settings/proxy_port", 3829);

    groupBox->setChecked(settings.value("settings/proxy_enabled", false).toBool());
    addressEdit->setText(settings.value("settings/proxy_address", QString()).toString());
    portSpin->setValue(settings.value("settings/proxy_port", 3829).toInt());
}
