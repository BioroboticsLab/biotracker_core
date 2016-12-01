#-------------------------------------------------
#
# Project created by QtCreator 2016-11-25T11:43:53
#
#-------------------------------------------------

QT       += widgets

TEMPLATE = lib
CONFIG  += plugin
TARGET   = $$qtLibraryTarget(IBioTrackerPlugin)

DEFINES += BIOTRACKERPLUGIN_LIBRARY

SOURCES += BioTrackerPlugin.cpp \
    Interfaces/IController/icontroller.cpp \
    Interfaces/IModel/imodel.cpp \
    Interfaces/IModel/ITrackedComponent.cpp \
    Interfaces/IModel/ITrackedComponentFactory.cpp \
    Interfaces/IModel/ITrackedTrajectory.cpp \
    Interfaces/IModel/ITrackingAlgorithm.cpp \
    Interfaces/IView/IGraphicsView.cpp \
    Interfaces/IView/IView.cpp \
    Interfaces/IView/IViewGraphicsScene.cpp \
    Interfaces/IView/IViewTrackedComponent.cpp \
    Interfaces/IBioTrackerContext.cpp \
    Model/TrackedComponents/TrackedComponentFactory.cpp \
    Model/TrackedComponents/TrackedElement.cpp \
    Model/TrackedComponents/TrackedTrajectory.cpp \
    Model/BioTrackerTrackingAlgorithm.cpp \
    Model/null_Model.cpp \
    Controller/ControllerTrackingAlgorithm.cpp \
    Controller/null_Controller.cpp


HEADERS += BioTrackerPlugin.h\
        biotrackerplugin_global.h \
    Interfaces/IController/icontroller.h \
    Interfaces/IModel/imodel.h \
    Interfaces/IModel/ITrackedComponent.h \
    Interfaces/IModel/ITrackedComponentFactory.h \
    Interfaces/IModel/ITrackedTrajectory.h \
    Interfaces/IModel/ITrackingAlgorithm.h \
    Interfaces/IView/IGraphicsView.h \
    Interfaces/IView/IView.h \
    Interfaces/IView/IViewGraphicsScene.h \
    Interfaces/IView/IViewTrackedComponent.h \
    Interfaces/ENUMS.h \
    Interfaces/IBioTrackerContext.h \
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
