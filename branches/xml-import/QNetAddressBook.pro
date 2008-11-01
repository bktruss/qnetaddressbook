TEMPLATE = app
TARGET = QNetAddressBook
VERSION = 0.0.1
CONFIG += release warn_on qt
QT += network sql core
DESTDIR = bin
MOC_DIR = build
OBJECTS_DIR = build
RCC_DIR = build
UI_DIR = build
VERSIONSTR = '\\"$${VERSION}\\" '
DEFINES += VERSION=\"$${VERSIONSTR}\"
DEPENDPATH += QMapControl resources src ui
INCLUDEPATH += QMapControl src
HEADERS += QMapControl/circlepoint.h \
 QMapControl/curve.h \
 QMapControl/geometry.h \
 QMapControl/geometrylayer.h \
 QMapControl/googlemapadapter.h \
 QMapControl/googlesatmapadapter.h \
 QMapControl/gps_position.h \
 QMapControl/imagemanager.h \
 QMapControl/imagepoint.h \
 QMapControl/layer.h \
 QMapControl/layermanager.h \
 QMapControl/linestring.h \
 QMapControl/mapadapter.h \
 QMapControl/mapcontrol.h \
 QMapControl/maplayer.h \
 QMapControl/mapnetwork.h \
 QMapControl/osmmapadapter.h \
 QMapControl/point.h \
 QMapControl/tilemapadapter.h \
 QMapControl/wmsmapadapter.h \
 QMapControl/yahoomapadapter.h \
 src/mainwindow.h \
 src/centralwidget.h \
 src/networkdialog.h \
 src/preferencespage.h \
 src/proxypreferencespage.h \
 src/cachepreferencespage.h \
 src/viewpreferencespage.h \
 src/preferencesdialog.h
SOURCES += QMapControl/circlepoint.cpp \
 QMapControl/curve.cpp \
 QMapControl/geometry.cpp \
 QMapControl/geometrylayer.cpp \
 QMapControl/googlemapadapter.cpp \
 QMapControl/googlesatmapadapter.cpp \
 QMapControl/gps_position.cpp \
 QMapControl/imagemanager.cpp \
 QMapControl/imagepoint.cpp \
 QMapControl/layer.cpp \
 QMapControl/layermanager.cpp \
 QMapControl/linestring.cpp \
 QMapControl/mapadapter.cpp \
 QMapControl/mapcontrol.cpp \
 QMapControl/maplayer.cpp \
 QMapControl/mapnetwork.cpp \
 QMapControl/osmmapadapter.cpp \
 QMapControl/point.cpp \
 QMapControl/tilemapadapter.cpp \
 QMapControl/wmsmapadapter.cpp \
 QMapControl/yahoomapadapter.cpp \
 src/main.cpp \
 src/mainwindow.cpp \
 src/centralwidget.cpp \
 src/networkdialog.cpp \
 src/preferencespage.cpp \
 src/proxypreferencespage.cpp \
 src/cachepreferencespage.cpp \
 src/viewpreferencespage.cpp \
 src/preferencesdialog.cpp
FORMS += ui/mainwindow.ui \
 ui/networkdialog.ui \
 ui/proxypreferencespage.ui \
 ui/cachepreferencespage.ui \
 ui/viewpreferencespage.ui \
 ui/preferencesdialog.ui
RESOURCES += resources/resources.qrc
mac {
 ICON =  resources/images/icon.icns
}
win32 {
 RC_FILE =  resources/icon.rc
}

