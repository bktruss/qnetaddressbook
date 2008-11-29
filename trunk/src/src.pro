TEMPLATE = app
TARGET = QNetAddressBook
VERSION = 0.0.1
QT += sql \
    network
DESTDIR = ../bin
MOC_DIR = ../build
OBJECTS_DIR = ../build
RCC_DIR = ../build
UI_DIR = ../build
VERSIONSTR = '\\"$${VERSION}\\" \
    '
DEFINES += VERSION=\"$${VERSIONSTR}\"
DEPENDPATH += . \
    ../QMapControl \
    ../resources \
    ../ui
INCLUDEPATH += . \
    ../QMapControl
LIBS += -L../bin/ \
    -lqmapcontrol
HEADERS += mainwindow.h \
    centralwidget.h \
    networkdialog.h \
    preferencespage.h \
    proxypreferencespage.h \
    cachepreferencespage.h \
    viewpreferencespage.h \
    preferencesdialog.h \
    searchdialog.h \
    interfaces.h \
    plugindialog.h
SOURCES += main.cpp \
    mainwindow.cpp \
    centralwidget.cpp \
    networkdialog.cpp \
    preferencespage.cpp \
    proxypreferencespage.cpp \
    cachepreferencespage.cpp \
    viewpreferencespage.cpp \
    preferencesdialog.cpp \
    searchdialog.cpp \
    plugindialog.cpp
FORMS += ../ui/mainwindow.ui \
    ../ui/networkdialog.ui \
    ../ui/proxypreferencespage.ui \
    ../ui/cachepreferencespage.ui \
    ../ui/viewpreferencespage.ui \
    ../ui/preferencesdialog.ui \
    ../ui/searchdialog.ui \
    ../ui/plugindialog.ui
RESOURCES += ../resources/resources.qrc
mac:ICON = ../resources/images/icon.icns
win32:RC_FILE = ../resources/icon.rc
