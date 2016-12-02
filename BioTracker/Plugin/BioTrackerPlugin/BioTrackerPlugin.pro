#-------------------------------------------------
#
# Project created by QtCreator 2016-11-25T11:43:53
#
#-------------------------------------------------

QT       += widgets

TEMPLATE = lib
CONFIG  += plugin
TARGET   = $$qtLibraryTarget(IBioTrackerPlugin)

INCLUDEPATH += ../../Interfaces/BioTrackerInterfaces/
LIBS += /home/andreas/Documents/biotracker_core/BioTracker/Interfaces/build-BioTrackerInterfaces-BioTracker-Debug/libBioTrackerInterfaces.a

DEFINES += BIOTRACKERPLUGIN_LIBRARY

SOURCES += BioTrackerPlugin.cpp \
    Model/TrackedComponents/TrackedComponentFactory.cpp \
    Model/TrackedComponents/TrackedElement.cpp \
    Model/TrackedComponents/TrackedTrajectory.cpp \
    Model/BioTrackerTrackingAlgorithm.cpp \
    Model/null_Model.cpp \
    Controller/ControllerTrackingAlgorithm.cpp \
    Controller/null_Controller.cpp


HEADERS += BioTrackerPlugin.h\
        biotrackerplugin_global.h \
    Model/TrackedComponents/TrackedComponentFactory.h \
    Model/TrackedComponents/TrackedElement.h \
    Model/TrackedComponents/TrackedTrajectory.h \
    Model/BioTrackerTrackingAlgorithm.h \
    Model/null_Model.h \
    Controller/ControllerTrackingAlgorithm.h \
    Controller/null_Controller.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    BioTrackerPlugin.json
