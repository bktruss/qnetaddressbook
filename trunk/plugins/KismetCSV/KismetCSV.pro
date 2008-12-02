TEMPLATE      = lib
CONFIG       += plugin
QT -= gui
INCLUDEPATH  += ../../src
HEADERS       = kismetcsv.h
SOURCES       = kismetcsv.cpp
TARGET        = $$qtLibraryTarget(importkismetcsv)
DESTDIR       = ../../bin/PlugIns
