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
#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QtGui/QDialog>
#include "ui_searchdialog.h"

class QSqlQueryModel;
class SearchDialog : public QDialog, Ui::SearchDialog
{
    Q_OBJECT

public:
    SearchDialog(QWidget *parent = 0);
    QSizeF selectedCoordinates() const; // maybe better with signal/slot

private slots:
    void performSearch();
    void networkSelected(const QModelIndex &index);

signals:
    void networkSelected(const QPointF &coordinate);

private:
    QSqlQueryModel *model;
};
#endif // SEARCHDIALOG_H
