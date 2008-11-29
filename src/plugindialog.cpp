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
#include <QPluginLoader>
#include <QHeaderView>
#include "interfaces.h"

#include "plugindialog.h"

PluginDialog::PluginDialog(QList<QPluginLoader *> pluginLoaders, QWidget *parent)
        : QDialog(parent), Ui::PluginDialog()
{
    setupUi(this);
    tableWidget->verticalHeader()->setShown(false);
    tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    populatePluginsList(pluginLoaders);
    tableWidget->resizeColumnsToContents();
}

void PluginDialog::populatePluginsList(QList<QPluginLoader *> pluginLoaders)
{
    foreach(QPluginLoader *loader, pluginLoaders){
        ImportInterface *plugin = qobject_cast<ImportInterface *>(loader->instance());
        if(plugin){
            int row = tableWidget->rowCount();
            tableWidget->insertRow(row);
            QTableWidgetItem *item = new QTableWidgetItem(plugin->name());
            tableWidget->setItem(row, 0, item);
            item = new QTableWidgetItem(plugin->author());
            tableWidget->setItem(row, 1, item);
            item = new QTableWidgetItem(plugin->description());
            tableWidget->setItem(row, 2, item);
        }
    }
}
