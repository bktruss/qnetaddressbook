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
#ifndef NETWORKDIALOG_H
#define NETWORKDIALOG_H

#include <QDialog>
#include "ui_networkdialog.h"

class NetworkDialog : public QDialog, public Ui::NetworkDialog
{
		Q_OBJECT

	public:
		typedef enum {
			NoAction = 0,
			SaveNetworkAction,
			RemoveNetworkAction	
		} Purpose;

		typedef enum {
			CreateMode = 0,
			ModifyMode,
		} Mode;
		
		NetworkDialog(Mode mode, QWidget *parent = 0);
		
		QString essid() const;
		void setEssid(const QString &essid);
		
		QString bssid() const;
		void setBssid(const QString &bssid);
				
		int channel() const;
		void setChannel(int channel);
		
		int signal() const;
		void setSignal(int signal);
		
		int encryption() const;
		void setEncryption(int encryption);
		
		double latitude() const;
		void setLatitude(double latitude);
		
		double longitude() const;
		void setLongitude(double longitude);
	
		QString comment() const;
		void setComment(const QString &comment);
	
		Purpose purpose() const;
		
	private slots:
		void validate();
		void setPurpose(QAbstractButton *button);
		
	private:
		Purpose m_purpose;
};
#endif
