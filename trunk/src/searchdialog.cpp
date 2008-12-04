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
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>
#include <QRegExp>

#include "networksmodel.h"

#include "searchdialog.h"

SearchDialog::SearchDialog(QWidget *parent) :
        QDialog(parent), Ui::SearchDialog()
{
    model = new NetworksModel(this);

    setupUi(this);
    checkBoxes = qFindChildren<QCheckBox *>(this, QRegExp(".+Check"));
    resultTable->setModel(model);
    resultTable->setVisible(false);
    connect(searchButton, SIGNAL(clicked()), this, SLOT(performSearch()));
    connect(resultTable, SIGNAL(activated(const QModelIndex &)), this, SLOT(networkSelected(const QModelIndex &)));
    foreach(QCheckBox *checkBox, checkBoxes){
        connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(validate()));
    }
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
        query.bindValue(":essid", essidEdit->text().replace('*', '%'));
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
    setupModel();
    resultTable->resizeColumnsToContents();
    resultTable->setVisible(true);
}

void SearchDialog::networkSelected(const QModelIndex &index)
{
    QPointF coordinate;
    coordinate.setX(model->record(index.row()).value("lon").toDouble());
    coordinate.setY(model->record(index.row()).value("lat").toDouble());
    emit networkSelected(coordinate);
}

void SearchDialog::validate()
{
    searchButton->setEnabled(false);
    foreach(QCheckBox *checkBox, checkBoxes){
        if(checkBox->isChecked()){
            searchButton->setEnabled(true);
            return;
        }
    }
}

void SearchDialog::setupModel()
{
    model->setHeaderData(0, Qt::Horizontal, tr("BSSID"));
    model->setHeaderData(1, Qt::Horizontal, tr("ESSID"));
    model->setHeaderData(2, Qt::Horizontal, tr("Channel"));
    model->setHeaderData(3, Qt::Horizontal, tr("Encryption"));
    model->setHeaderData(4, Qt::Horizontal, tr("Signal"));
    model->setHeaderData(5, Qt::Horizontal, tr("Longitude"));
    model->setHeaderData(6, Qt::Horizontal, tr("Latitude"));
}
