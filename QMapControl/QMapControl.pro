TEMPLATE = lib
TARGET = qmapcontrol
QT += network
VERSION = 0.9.6
DESTDIR = ../bin
MOC_DIR = ../build
OBJECTS_DIR = ../build
RCC_DIR = ../build
UI_DIR = ../build

DEPENDPATH += .
INCLUDEPATH += .

HEADERS += circlepoint.h \
           curve.h \
           geometry.h \
           geometrylayer.h \
           googlemapadapter.h \
           googlesatmapadapter.h \
           gps_position.h \
           imagemanager.h \
           imagepoint.h \
           layer.h \
           layermanager.h \
           linestring.h \
           mapadapter.h \
           mapcontrol.h \
           maplayer.h \
           mapnetwork.h \
           osmmapadapter.h \
           point.h \
           tilemapadapter.h \
           wmsmapadapter.h \
           yahoomapadapter.h
SOURCES += circlepoint.cpp \
           curve.cpp \
           geometry.cpp \
           geometrylayer.cpp \
           googlemapadapter.cpp \
           googlesatmapadapter.cpp \
           gps_position.cpp \
           imagemanager.cpp \
           imagepoint.cpp \
           layer.cpp \
           layermanager.cpp \
           linestring.cpp \
           mapadapter.cpp \
           mapcontrol.cpp \
           maplayer.cpp \
           mapnetwork.cpp \
           osmmapadapter.cpp \
           point.cpp \
           tilemapadapter.cpp \
           wmsmapadapter.cpp \
           yahoomapadapter.cpp
macx {
    QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
    CONFIG += x86 ppc
}
