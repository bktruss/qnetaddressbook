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
#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>
#include <QString>
#include <QIODevice>

struct Network {
    QString essid;
    QString bssid;
    int channel;
    int signal;
    double lat;
    double lon;
    QString comment;
    int encryption;
};

class ImportInterface
{
public:
    virtual ~ImportInterface() {}

    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual QString author() const = 0;
    virtual QString fileNameFilter() const = 0;
    virtual QList<Network> importNetworks(QIODevice &device) = 0;
    virtual QString errorText() const = 0;
};

Q_DECLARE_INTERFACE(ImportInterface, "com.googlecode.QNetAddressBook.ImportInterface/1.0");
#endif // INTERFACES_H
