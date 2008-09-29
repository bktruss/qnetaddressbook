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
#include "networkdialog.h"

NetworkDialog::NetworkDialog( QWidget *parent) 
	: QDialog(parent), Ui::NetworkDialog()
{
	setupUi(this);
}

QString NetworkDialog::essid() const 
{
	return essidEdit->text();
}

void NetworkDialog::setEssid(const QString &essid)
{
	essidEdit->setText(essid);
}

QString NetworkDialog::bssid() const 
{
	return bssidEdit->text();
}

void NetworkDialog::setBssid(const QString &bssid)
{
	bssidEdit->setText(bssid);
}

int NetworkDialog::channel() const 
{
	return channelSpin->value();
}

void NetworkDialog::setChannel(int channel)
{
	channelSpin->setValue(channel);
}

int NetworkDialog::signal() const
{
	return signalSpin->value();	
}

void NetworkDialog::setSignal(int signal)
{
	signalSpin->setValue(signal);	
}

int NetworkDialog::encryption() const 
{
	return encryptionCombo->currentIndex();
}

void NetworkDialog::setEncryption(int encryption)
{
	encryptionCombo->setCurrentIndex(encryption);
}

double NetworkDialog::latitude() const 
{
	return latitudeSpin->value();
}

void NetworkDialog::setLatitude(double latitude)
{
	latitudeSpin->setValue(latitude);
}

double NetworkDialog::longitude() const 
{
	return longitudeSpin->value();
}

void NetworkDialog::setLongitude(double longitude)
{
	longitudeSpin->setValue(longitude);
}

QString NetworkDialog::comment() const 
{
	return commentEdit->toPlainText();
}

void NetworkDialog::setComment(const QString &comment)
{
	commentEdit->setPlainText(comment);
}
