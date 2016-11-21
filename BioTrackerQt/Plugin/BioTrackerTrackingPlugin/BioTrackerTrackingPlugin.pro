#-------------------------------------------------
#
# Project created by QtCreator 2016-11-20T10:58:44
#
#-------------------------------------------------

QT       += widgets

TARGET = BioTrackerTrackingPlugin
TEMPLATE = lib

DEFINES += BIOTRACKERTRACKINGPLUGIN_LIBRARY

SOURCES += BioTrackerTrackingPlugin.cpp

HEADERS += BioTrackerTrackingPlugin.h\
        biotrackertrackingplugin_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
