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
#ifndef NETSTUMBLER_H
#define NETSTUMBLER_H

#include <QObject>

#include "interfaces.h"

class QIODevice;
class QDataStream;
class Netstumbler : public QObject, public ImportInterface
{
    Q_OBJECT
    Q_INTERFACES(ImportInterface)
            
public:
    Netstumbler(QObject *parent = 0);

    QString name() const;
    QString description() const;
    QString author() const;
    QString fileNameFilter() const;
    QList<Network> importNetworks(QIODevice &device);
    QString errorText() const;

private:
    bool importNetwork(QDataStream &stream, Network *network);
    QString error;
};
#endif
