TEMPLATE      = lib
CONFIG       += plugin
VERSION       = "0.9.9"
QT -= gui
INCLUDEPATH  += ../../src
HEADERS       = kismetcsv.h
SOURCES       = kismetcsv.cpp
TARGET        = $$qtLibraryTarget(importkismetcsv)
DESTDIR       = ../../bin/PlugIns
MOC_DIR       = ../../build
OBJECTS_DIR   = ../../build
RCC_DIR       = ../../build
macx {
    QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
    CONFIG += x86 ppc
}
