TEMPLATE      = lib
CONFIG       += plugin
QT -= gui
INCLUDEPATH  += ../../src
HEADERS       = kismetcsv.h
SOURCES       = kismetcsv.cpp
TARGET        = $$qtLibraryTarget(importkismetcsv)
DESTDIR       = ../../bin/PlugIns
mac {
    QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
    CONFIG += x86 ppc
}
