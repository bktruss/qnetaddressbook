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
#include <QDir>
#include <QFileDialog>

#include "cachepreferencespage.h"

CachePreferencesPage::CachePreferencesPage( QWidget *parent ) 
    : PreferencesPage(parent), Ui::CachePreferencesPage()
{
    setupUi(this);

    groupBox->setChecked(settings.value("settings/cache_enabled", false).toBool());
    cacheEdit->setText(settings.value("settings/cache_dir", QDir::homePath() + "/QMapControl.cache").toString());

    connect(chooseButton, SIGNAL(clicked()), this, SLOT(chooseDirectory()));
    connect(groupBox, SIGNAL(toggled(bool)), this, SIGNAL(settingsChanged()));
    connect(cacheEdit, SIGNAL(textChanged(const QString &)), this, SIGNAL(settingsChanged()));
}

void CachePreferencesPage::applyChanges()
{
    settings.setValue("settings/cache_enabled", groupBox->isChecked());
    settings.setValue("settings/cache_dir", cacheEdit->text());
}

void CachePreferencesPage::restoreDefaults()
{
    settings.setValue("settings/cache_enabled", false);
    settings.setValue("settings/cache_dir", QDir::homePath() + "/QMapControl.cache");

    groupBox->setChecked(settings.value("settings/cache_enabled", false).toBool());
    cacheEdit->setText(settings.value("settings/cache_dir", QDir::homePath() + "/QMapControl.cache").toString());
}

void CachePreferencesPage::chooseDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose Cache Directory"), QDir::homePath(), QFileDialog::ShowDirsOnly);
    if (!dir.isEmpty())
        cacheEdit->setText(dir);
}
