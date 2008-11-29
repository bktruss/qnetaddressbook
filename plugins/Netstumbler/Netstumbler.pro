TEMPLATE      = lib
CONFIG       += plugin
QT -= gui
INCLUDEPATH  += ../../src
HEADERS       = netstumbler.h
SOURCES       = netstumbler.cpp
TARGET        = $$qtLibraryTarget(importnetstumbler)
DESTDIR       = ../../bin/plugins
