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
#include <QDataStream>
#include <QStringList>

#include "netstumbler.h"


Netstumbler::Netstumbler (QObject *parent) : QObject(parent), ImportInterface()
{

}

QString Netstumbler::name() const
{
    return tr("Netstumbler Log");
}

QString Netstumbler::description() const
{
    return tr("Plug-in for import of Netstumbler log files");
}

QString Netstumbler::author() const
{
    return QString("Lorenzo \"Il Rugginoso\" Masini <lorenxo86@gmail.com>");
}

QString Netstumbler::fileNameFilter() const
{
    return tr("Netstumbler log files (*.ns1)");
}

QList<Network> Netstumbler::importNetworks(QIODevice &device)
{
    QList<Network> networks;
    /* NS1 File format http://www.stumbler.net/ns1files.html */
    QDataStream stream(&device);
    stream.setByteOrder(QDataStream::LittleEndian);

    char *signature = new char[5];
    quint32 version;

    stream.readRawData(signature, 4);
    signature[4] = 0;

    stream >> version;

    /* Verify singature and file version */
    if (QString(signature) != "NetS" || version != 12){
        delete[] signature;
        return networks;
    }
    quint32 apCount;
    stream >> apCount;  // Get the number of entries
    for (quint32 i = 0; i < apCount; i++){
        Network network;
        if(importNetwork(stream, &network))
            networks.append(network);
    }
    return networks;
}

QString Netstumbler::errorText() const
{
    return error;
}

bool Netstumbler::importNetwork(QDataStream &stream, Network *network)
{
    bool error = false;
    // SSID
    quint8 ssidLength = 0;
    stream >> ssidLength;
    char *rawSsid = new char[ssidLength + 1];
    stream.readRawData(rawSsid, ssidLength);
    rawSsid[ssidLength] = 0;
    QString SSID = rawSsid;
    delete[] rawSsid;
    if(SSID.isEmpty())
        error = true;

    // BSSID
    quint8 rawBssid[6];
    QStringList bssidFields;
    for (int i = 0; i < 6; i++){
        stream >> rawBssid[i];
        bssidFields.append(QString("%1").arg(rawBssid[i], 2, 16, QLatin1Char('0')).toUpper());
    }
    QString BSSID = bssidFields.join(":");

    // SIGNAL
    qint32 signal;
    stream >> signal;
    signal = abs(signal);
    stream.skipRawData(8);

    // Encryption
    quint32 flags;
    stream >> flags;
    int encryption;
    if (flags & 0x0010) // bitwise AND between flags and encrpytion mask 0x0010
        encryption = 0;
    else
        encryption = 1;

    // Skip useless data
    stream.skipRawData(20);

    // Coordinates
    double latitude;
    double longitude;
    stream >> latitude >> longitude;
    if (latitude == 0 || longitude == 0)
        error = true;

    // Skip useless data
    quint32 dataCount;
    stream >> dataCount;
    for (quint32 i = 0; i < dataCount; i++){
        stream.skipRawData(16);
        qint32 locationSource;
        stream >> locationSource;
        if (locationSource)
            stream.skipRawData(60);
    }

    // Skip useless data
    quint8 nameLength;
    stream >> nameLength;
    stream.skipRawData(nameLength + 8);

    // Channel
    quint32 channel;
    stream >> channel;

    // Skip useless data
    stream.skipRawData(28);
    quint32 ieLength;
    stream >> ieLength;
    for(quint32 i = 0; i < ieLength; i++)
        stream.skipRawData(1);

    if (error)
        return false;
    else {
        network->essid = SSID;
        network->bssid = BSSID;
        network->channel = channel;
        network->signal = signal;
        network->lat = latitude;
        network->lon = longitude;
        network->comment = QString();
        network->encryption = encryption;
        return true;
    }
}
Q_EXPORT_PLUGIN2(importnetstumbler, Netstumbler)      
