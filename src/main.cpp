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
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>

#include "mainwindow.h"

#ifdef Q_WS_MAC
#include <QDir>
#endif

int main(int argc, char ** argv)
{
    QCoreApplication::setOrganizationName("qnetaddressbook");
    QCoreApplication::setOrganizationDomain("googlecode.com");
    QCoreApplication::setApplicationName("QNetAddressBook");

    QApplication app( argc, argv );
#ifdef Q_WS_MAC
    QDir dir(QApplication::applicationDirPath());
    dir.cdUp();
    dir.cd("plugins");
    QCoreApplication::addLibraryPath(dir.absolutePath());
#endif
    app.setWindowIcon(QIcon(":/images/icon.png"));

    QSplashScreen splash(QPixmap(":images/splash.png"));
    splash.show();
    splash.showMessage(QObject::tr("Loading Main Window..."), Qt::AlignRight | Qt::AlignBottom, Qt::white);
    app.processEvents();

    MainWindow win;
    win.show();

    splash.showMessage(QObject::tr("Starting up..."), Qt::AlignRight | Qt::AlignBottom, Qt::white);
    app.processEvents();
    splash.finish(&win);

    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    return app.exec();
}
