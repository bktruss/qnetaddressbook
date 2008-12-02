TEMPLATE      = lib
CONFIG       += plugin
QT -= gui
INCLUDEPATH  += ../../src
HEADERS       = netstumbler.h
SOURCES       = netstumbler.cpp
TARGET        = $$qtLibraryTarget(importnetstumbler)
DESTDIR       = ../../bin/PlugIns
mac {
    QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
    CONFIG += x86 ppc
}
