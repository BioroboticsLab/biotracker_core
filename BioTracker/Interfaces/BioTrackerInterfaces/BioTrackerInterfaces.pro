#-------------------------------------------------
#
# Project created by QtCreator 2016-12-01T17:31:27
#
#-------------------------------------------------

QT       += widgets

TARGET = BioTrackerInterfaces
TEMPLATE = lib
CONFIG += staticlib

SOURCES += Interfaces/IController/icontroller.cpp \
    Interfaces/IController/IControllerStrategy.cpp \
    Interfaces/IModel/imodel.cpp \
    Interfaces/IModel/IObject.cpp \
    Interfaces/IModel/ITrackedComponent.cpp \
    Interfaces/IModel/ITrackedComponentFactory.cpp \
    Interfaces/IModel/ITrackedTrajectory.cpp \
    Interfaces/IModel/ITrackingAlgorithm.cpp \
    Interfaces/IView/IGraphicsPixmapItem.cpp \
    Interfaces/IView/IGraphicsView.cpp \
    Interfaces/IView/IView.cpp \
    Interfaces/IView/IViewGraphicsScene.cpp \
    Interfaces/IView/IViewMainWindow.cpp \
    Interfaces/IView/IViewOpenGLWidget.cpp \
    Interfaces/IView/IViewTrackedComponent.cpp \
    Interfaces/IView/IViewWidget.cpp \
    Interfaces/IBioTrackerContext.cpp

HEADERS += Interfaces/IController/icontroller.h \
    Interfaces/IController/IControllerStrategy.h \
    Interfaces/IModel/imodel.h \
    Interfaces/IModel/IObject.h \
    Interfaces/IModel/ITrackedComponent.h \
    Interfaces/IModel/ITrackedComponentFactory.h \
    Interfaces/IModel/ITrackedTrajectory.h \
    Interfaces/IModel/ITrackingAlgorithm.h \
    Interfaces/IView/IGraphicsPixmapItem.h \
    Interfaces/IView/IGraphicsView.h \
    Interfaces/IView/IView.h \
    Interfaces/IView/IViewGraphicsScene.h \
    Interfaces/IView/IViewMainWindow.h \
    Interfaces/IView/IViewOpenGLWidget.h \
    Interfaces/IView/IViewTrackedComponent.h \
    Interfaces/IView/IViewWidget.h \
    Interfaces/ENUMS.h \
    Interfaces/IBioTrackerContext.h \
    Interfaces/IBioTrackerPlugin.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
