/***************************************************************************
 *   Copyright (C) 2008 by Lorenzo Masini                                  *
 *   lorenxo86@gmail.com                                                   *
 *   Copyright (C) 2008 by Andrea Decorte                                  *
 *   adecorte@gmail.com                                                    *
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
#include <QSettings>
#include <QMessageBox>
#include <QDir>
#include <QPluginLoader>
#include <QFileDialog>
#include <QKeySequence>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "mapadapter.h"
#include "googlemapadapter.h"
#include "layer.h"
#include "maplayer.h"

#include "mainwindow.h"
#include "centralwidget.h"
#include "searchdialog.h"
#include "preferencesdialog.h"
#include "interfaces.h"
#include "plugindialog.h"

MainWindow::MainWindow( QWidget * parent, Qt::WFlags f) 
    : QMainWindow(parent, f)
{
    QSettings settings;
    setupUi(this);

    w = new CentralWidget(this);
    this->setCentralWidget(w);

    statusLabel.append(new QLabel(tr("Lon: +000.000000")));
    statusLabel.append(new QLabel(tr("Lat: +00.0000000")));
    statusLabel.append(new QLabel(tr("Zoom: 00")));
    for (int i = 0; i <= 2; i++){
        statusLabel[i]->setMinimumSize(statusLabel[i]->sizeHint());
        statusLabel[i]->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        statusBar()->addPermanentWidget(statusLabel[i]);
    }

    connect(w, SIGNAL(viewChanged(const QPointF &, int)), this, SLOT(updateStatusBar(const QPointF &, int)));

    setupActions();
    setActionsEnabled(false);
#ifdef Q_WS_MAC
    setUnifiedTitleAndToolBarOnMac(true);
    statusBar()->setSizeGripEnabled(false);
#else
    restoreState(settings.value("mainwindow/state").toByteArray());
#endif
    restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
}

void MainWindow::loadPlugins()
{
    QDir pluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader *loader = new QPluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader->instance();
        if (plugin) {
            populateMenus(plugin);
            pluginLoaders.append(loader);
        } else
            delete loader;
    }
    menuImport->setHidden(menuImport->actions().isEmpty());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    closeFile();
    settings.setValue("mainwindow/geometry", saveGeometry());
#ifndef Q_WS_MAC
    settings.setValue("mainwindow/state", saveState());
#endif
    QMainWindow::closeEvent(event);
}

void MainWindow::newFile()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("New Network Databse"), QDir::homePath() + QDir::separator() + tr("untitled.db"), tr("Network Database (*.db)"));
    if(filename.isEmpty())
        return;

    QSqlDatabase::database().close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filename);
    if(!db.open()){
        QMessageBox::warning(this, db.lastError().driverText(), db.lastError().databaseText());
        return;
    }

    db.transaction();
    QSqlQuery tableQuery;
    if(!tableQuery.exec("CREATE TABLE \"networks\" (\"essid\" TEXT NOT NULL, \"bssid\" TEXT NOT NULL, \"channel\" INTEGER NOT NULL, \"signal\" INTEGER NOT NULL, \"lat\" REAL NOT NULL, \"lon\" REAL NOT NULL, \"comment\" TEXT, \"encryption\" INTEGER  NOT NULL  DEFAULT (0))"))
        db.rollback();

    QSqlQuery keyQuery;
    if(!tableQuery.exec("CREATE UNIQUE INDEX \"bssid\" on networks (bssid ASC)"))
        db.rollback();
    db.commit();

    w->clearNetworks();
    setActionsEnabled(true);
}

void MainWindow::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Network Database File"), QString(), tr("Network Database (*.db)"));
    if(filename.isEmpty())
        return;

    QSqlDatabase::database().close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filename);
    db.open();
    w->clearNetworks();
    w->loadNetworks();
    setActionsEnabled(true);
}

void MainWindow::closeFile()
{
    QSqlDatabase::database().close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    w->clearNetworks();
    setActionsEnabled(false);
}

void MainWindow::showFindDialog()
{
    SearchDialog dialog(this);
    connect(&dialog, SIGNAL(networkSelected(const QPointF &)), w, SLOT(moveTo(const QPointF &)));
    dialog.exec();
}

void MainWindow::showPreferencesDialog()
{
    PreferencesDialog dialog(this);
    dialog.exec();
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, tr("About NetAddressBook"), tr("<h1>NetAddressBook "VERSION"</h1>"
                                                            "<h3>An addressbook for wireless networks</h3>"
                                                            "<small>Copyleft 2008 Lorenzo Masini</small>"
                                                            "<h3>Authors:</h3>"
                                                            "<small><ul>"
                                                            "<li>Lorenzo \"Il Rugginoso\" Masini &lt;<a href=\"mailto:lorenxo86@gmail.com\">lorenxo86@gmail.com</a>&gt;</li>"
                                                            "<li>Andrea \"Klenje\" Decorte &lt;<a href=\"mailto:adecorte@gmail.com\">adecorte@gmail.com</a>&gt;</li>"
                                                            "</ul></small>"));
}

void MainWindow::showAboutPlugins()
{
    PluginDialog dialog(pluginLoaders, this);
    dialog.exec();
}

void MainWindow::updateStatusBar(const QPointF &coordinate, int zoom)
{
    statusLabel[0]->setText(tr("Lon: %1").arg(coordinate.x(), 0, 'f', 6));
    statusLabel[1]->setText(tr("Lat: %1").arg(coordinate.y(), 0, 'f', 6));
    statusLabel[2]->setText(tr("Zoom: %1").arg(zoom));
}

void MainWindow::importNetworks()
{
    QAction *action = qobject_cast<QAction *>(sender());
    ImportInterface *importInterface = qobject_cast<ImportInterface *>(action->parent());

    QString filename = QFileDialog::getOpenFileName(this, tr("Import Networks"), QString(), importInterface->fileNameFilter());
    if(filename.isEmpty())
        return;
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly)){
        QList<Network> networks = importInterface->importNetworks(file);
        if(!importInterface->errorText().isEmpty()){
            QMessageBox::warning(this, tr("Error importing networks"), importInterface->errorText());
            return;
        }
        w->importNetworks(networks);
    }
    file.close();
}

void MainWindow::setupActions()
{
    /* File */
    actionNew->setStatusTip(tr("Creates a new network database"));
    actionNew->setShortcut(QKeySequence(QKeySequence::New));
    connect(actionNew, SIGNAL(triggered()), this, SLOT(newFile()));

    actionOpen->setStatusTip(tr("Opens a network database"));
    actionOpen->setShortcut(QKeySequence(QKeySequence::Open));
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));

    actionClose->setStatusTip(tr("Closes the network database"));
    actionClose->setShortcut(QKeySequence(QKeySequence::Close));
    connect(actionClose, SIGNAL(triggered()), this, SLOT(closeFile()));

    actionQuit->setStatusTip(tr("Quits the application"));
    actionQuit->setShortcut(QKeySequence("Ctrl+Q"));
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    /* Edit */
    actionAddNetwork->setStatusTip(tr("Adds a network in the centre of the map"));
    connect(actionAddNetwork, SIGNAL(triggered()), w, SLOT(addNetwork()));

    actionFind->setStatusTip(tr("Shows the find dialog"));
    connect(actionFind, SIGNAL(triggered()), this, SLOT(showFindDialog()));

    actionPreferences->setStatusTip(tr("Opens preferences dialog"));
    connect(actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferencesDialog()));

    /* View */
    actionZoomIn->setStatusTip(tr("Zooms in the map"));
    actionZoomIn->setShortcut(QKeySequence(QKeySequence::ZoomIn));
    connect(actionZoomIn, SIGNAL(triggered()), w, SLOT(zoomIn()));

    actionZoomOut->setStatusTip(tr("Zooms out the map"));
    actionZoomOut->setShortcut(QKeySequence(QKeySequence::ZoomOut));
    connect(actionZoomOut, SIGNAL(triggered()), w, SLOT(zoomOut()));

    actionOpenNetworks->setStatusTip(tr("Shows/Hides Open Networks"));
    actionOpenNetworks->setChecked(true);
    connect(actionOpenNetworks, SIGNAL(toggled(bool)), w, SLOT(setVisibleOpenNetworks(bool)));

    actionWEPNetworks->setStatusTip(tr("Shows/Hides WEP Encrypted Networks"));
    actionWEPNetworks->setChecked(true);
    connect(actionWEPNetworks, SIGNAL(toggled(bool)), w, SLOT(setVisibleWEPNetworks(bool)));

    actionWPANetworks->setStatusTip(tr("Shows/Hides WPA Encrypted Networks"));
    actionWPANetworks->setChecked(true);
    connect(actionWPANetworks, SIGNAL(toggled(bool)), w, SLOT(setVisibleWPANetworks(bool)));

    actionWPA2Networks->setStatusTip(tr("Shows/Hides WPA2 Encrypted Networks"));
    actionWPA2Networks->setChecked(true);
    connect(actionWPA2Networks, SIGNAL(toggled(bool)), w, SLOT(setVisibleWPA2Networks(bool)));

    /* Go */
    actionGoHome->setStatusTip(tr("Changes coordinates/zoom to the initial ones"));
    actionGoHome->setShortcut(QKeySequence("Ctrl+h"));
    connect(actionGoHome, SIGNAL(triggered()), w, SLOT(goHome()));

    /* Help */
    actionAbout->setStatusTip(tr("Shows the about dialog"));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));

    actionAboutPlugins->setStatusTip(tr("Shows the plug-ins dialog"));
    connect(actionAboutPlugins, SIGNAL(triggered()), this, SLOT(showAboutPlugins()));
}

void MainWindow::setActionsEnabled(bool enabled)
{
    /* File */
    actionNew->setEnabled(!enabled);
    actionOpen->setEnabled(!enabled);
    actionClose->setEnabled(enabled);
    menuImport->setEnabled(enabled);

    /* Edit */
    actionAddNetwork->setEnabled(enabled);
    actionFind->setEnabled(enabled);

    /* View */
    actionOpenNetworks->setEnabled(enabled);
    actionWEPNetworks->setEnabled(enabled);
    actionWPANetworks->setEnabled(enabled);
    actionWPA2Networks->setEnabled(enabled);
}

void MainWindow::populateMenus(QObject *plugin)
{
    ImportInterface *importInterface = qobject_cast<ImportInterface *>(plugin);
    if (importInterface)
        addToMenu(plugin, importInterface->name(), menuImport, SLOT(importNetworks()));
}

void MainWindow::addToMenu(QObject *plugin, const QString &text, QMenu *menu, const char *member)
{
    QAction *action = new QAction(text, plugin);
    connect(action, SIGNAL(triggered()), this, member);
    menu->addAction(action);
}
