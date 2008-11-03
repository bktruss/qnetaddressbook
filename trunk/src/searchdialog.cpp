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
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>

#include "searchdialog.h"

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent), Ui::SearchDialog()
{
    model = new QSqlQueryModel(this);

    setupUi(this);
    resultTable->setModel(model);
    resultTable->setVisible(false);
    connect(searchButton, SIGNAL(clicked()), this, SLOT(performSearch()));
    connect(resultTable, SIGNAL(activated(const QModelIndex &)), this, SLOT(networkSelected(const QModelIndex &)));
}

void SearchDialog::performSearch()
{
    model->clear();
    QSqlQuery query;
    QString queryString = "SELECT bssid, essid, channel, encryption, signal, lon, lat FROM networks WHERE ";
    QStringList queryFields;
    if (bssidCheck->isChecked())
        queryFields.append("bssid = :bssid");
    if (essidCheck->isChecked())
        queryFields.append("essid LIKE :essid");
    if (channelCheck->isChecked())
        queryFields.append("channel = :channel");
    if (encryptionCheck->isChecked())
        queryFields.append("encryption = :encryption");
    if (signalCheck->isChecked())
        queryFields.append("signal >= :signal");

    queryString.append(queryFields.join(" AND "));

    query.prepare(queryString);
    if (bssidCheck->isChecked())
        query.bindValue(":bssid", bssidEdit->text());
    if (essidCheck->isChecked())
        query.bindValue(":essid", essidEdit->text());
    if (channelCheck->isChecked())
        query.bindValue(":channel", channelSpin->value());
    if (encryptionCheck->isChecked())
        query.bindValue(":encryption", encryptionCombo->currentIndex());
    if (signalCheck->isChecked())
        query.bindValue(":signal", signalSpin->value());

    if(!query.exec()){
        QMessageBox::critical(this, query.lastError().driverText(), query.lastError().databaseText());
        return;
    }

    model->setQuery(query);
    resultTable->setVisible(true);
}

void SearchDialog::networkSelected(const QModelIndex &index)
{
    QPointF coordinate;
    coordinate.setX(model->record(index.row()).value("lon").toDouble());
    coordinate.setY(model->record(index.row()).value("lat").toDouble());
    emit networkSelected(coordinate);
}
