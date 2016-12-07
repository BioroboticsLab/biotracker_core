#-------------------------------------------------
#
# Project created by QtCreator 2016-12-01T17:31:27
#
#-------------------------------------------------

QT       += widgets

TARGET = BioTrackerInterfaces
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    Interfaces/IModel/IObject.cpp \
    Interfaces/IView/IView.cpp \
    Interfaces/IView/IViewGraphicsScene.cpp \
    Interfaces/IView/IViewMainWindow.cpp \
    Interfaces/IView/IViewOpenGLWidget.cpp \
    Interfaces/IView/IViewTrackedComponent.cpp \
    Interfaces/IView/IViewWidget.cpp \
    Interfaces/IBioTrackerContext.cpp \
    Interfaces/IController/IController.cpp \
    Interfaces/IModel/IModel.cpp \
    Interfaces/IView/IViewGraphicsPixmapItem.cpp \
    Interfaces/IView/IViewGraphicsView.cpp \
    Interfaces/IModel/IModelTrackedComponent.cpp \
    Interfaces/IModel/IModelTrackedComponentFactory.cpp \
    Interfaces/IModel/IModelTrackedTrajectory.cpp \
    Interfaces/IModel/IModelTrackingAlgorithm.cpp

HEADERS += \
    Interfaces/IModel/IObject.h \
    Interfaces/IView/IView.h \
    Interfaces/IView/IViewGraphicsScene.h \
    Interfaces/IView/IViewMainWindow.h \
    Interfaces/IView/IViewOpenGLWidget.h \
    Interfaces/IView/IViewTrackedComponent.h \
    Interfaces/IView/IViewWidget.h \
    Interfaces/ENUMS.h \
    Interfaces/IBioTrackerContext.h \
    Interfaces/IBioTrackerPlugin.h \
    Interfaces/IController/IController.h \
    Interfaces/IModel/IModel.h \
    Interfaces/IView/IViewGraphicsPixmapItem.h \
    Interfaces/IView/IViewGraphicsView.h \
    Interfaces/IModel/IModelTrackedComponent.h \
    Interfaces/IModel/IModelTrackedComponentFactory.h \
    Interfaces/IModel/IModelTrackedTrajectory.h \
    Interfaces/IModel/IModelTrackingAlgorithm.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
