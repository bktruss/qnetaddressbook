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
#include <QTextStream>
#include <QStringList>

#include "kismetcsv.h"


KismetCSV::KismetCSV (QObject *parent) : QObject(parent), ImportInterface()
{

}

QString KismetCSV::name() const
{
    return tr("Kismet CSV Log");
}

QString KismetCSV::description() const
{
    return tr("Plug-in for import of Kismet CSV log files");
}

QString KismetCSV::author() const
{
    return QString("Lorenzo \"Il Rugginoso\" Masini <lorenxo86@gmail.com>");
}

QString KismetCSV::fileNameFilter() const
{
    return tr("Kismet CSV Files (*.csv)");
}

QList<Network> KismetCSV::importNetworks(QIODevice &device)
{
    QTextStream stream(&device);
    QList<Network> networks;
    if (stream.readLine() != "Network;NetType;ESSID;BSSID;Info;Channel;Cloaked;Encryption;Decrypted;MaxRate;MaxSeenRate;Beacon;LLC;Data;Crypt;Weak;Total;Carrier;Encoding;FirstTime;LastTime;BestQuality;BestSignal;BestNoise;GPSMinLat;GPSMinLon;GPSMinAlt;GPSMinSpd;GPSMaxLat;GPSMaxLon;GPSMaxAlt;GPSMaxSpd;GPSBestLat;GPSBestLon;GPSBestAlt;DataSize;IPType;IP;"){
        error = tr("Invalid file format.");
        return networks;
    }
    while(!stream.atEnd()){
        QString line = stream.readLine();
        Network network;
        if(importNetwork(line, &network))
            networks.append(network);
    }
    return networks;
}

QString KismetCSV::errorText() const
{
    return QString();
}

bool KismetCSV::importNetwork(const QString &line, Network *network)
{
    QStringList fields = line.split(";");
    int encryption = 0;

    if(fields.at(1) != "infrastructure")
        return false;

    if(fields.at(2) == "<no ssid>")
        return false;

    if(fields.at(7).contains("None"))
        encryption = 0;

    if(fields.at(7).contains("WEP"))
        encryption = 1;

    if(fields.at(7).contains("TKIP"))
        encryption = 2;

    if(fields.at(7).contains("AES-CCM"))
        encryption = 3;

    network->essid = fields.at(2);
    network->bssid = fields.at(3);
    network->channel = fields.at(5).toInt();
    network->signal = fields.at(22).toInt();
    network->lat = fields.at(32).toDouble();
    network->lon = fields.at(33).toDouble();
    network->comment = QString();
    network->encryption = encryption;

    return true;
}
Q_EXPORT_PLUGIN2(importkismetcsv, KismetCSV)
