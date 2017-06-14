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
LIBS += ../../Interfaces/build-BioTrackerInterfaces-Desktop-Debug/libBioTrackerInterfaces.a

DEFINES += BIOTRACKERPLUGIN_LIBRARY

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib \
-lopencv_core \
-lopencv_imgproc \
-lopencv_highgui \
-lopencv_ml \
-lopencv_video \
-lopencv_features2d \
-lopencv_calib3d \
-lopencv_objdetect \
-lopencv_contrib \
-lopencv_legacy \
-lopencv_flann

SOURCES += BioTrackerPlugin.cpp \
    Model/TrackedComponents/TrackedComponentFactory.cpp \
    Model/TrackedComponents/TrackedElement.cpp \
    Model/TrackedComponents/TrackedTrajectory.cpp \
    Model/BioTrackerTrackingAlgorithm.cpp \
    Model/null_Model.cpp \
    Controller/ControllerTrackingAlgorithm.cpp \
    Controller/null_Controller.cpp \
    Controller/ControllerTrackedComponent.cpp \
    View/TrackedElementView.cpp \
    Model/TrackerParameter.cpp \
    View/TrackerParameterView.cpp


HEADERS += BioTrackerPlugin.h\
        biotrackerplugin_global.h \
    Model/TrackedComponents/TrackedComponentFactory.h \
    Model/TrackedComponents/TrackedElement.h \
    Model/TrackedComponents/TrackedTrajectory.h \
    Model/BioTrackerTrackingAlgorithm.h \
    Model/null_Model.h \
    Controller/ControllerTrackingAlgorithm.h \
    Controller/null_Controller.h \
    Controller/ControllerTrackedComponent.h \
    View/TrackedElementView.h \
    Model/TrackerParameter.h \
    View/TrackerParameterView.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    BioTrackerPlugin.json

FORMS += \
    View/TrackerParameterView.ui
