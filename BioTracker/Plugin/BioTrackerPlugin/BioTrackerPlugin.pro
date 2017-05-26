#-------------------------------------------------
#
# Project created by QtCreator 2016-11-25T11:43:53
#
#-------------------------------------------------

QT       += widgets

TEMPLATE = lib
CONFIG  += plugin
TARGET   = $$qtLibraryTarget(IBioTrackerPlugin)

CONFIG += c++11
QMAKE_CFLAGS = -fpermissive
QMAKE_CXXFLAGS = -fpermissive
QMAKE_LFLAGS = -fpermissive


INCLUDEPATH += ../../Interfaces/BioTrackerInterfaces/
LIBS += ../../Interfaces/build-BioTrackerInterfaces-Desktop-Debug/libBioTrackerInterfaces.a

DEFINES += BIOTRACKERPLUGIN_LIBRARY

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib \
-lopencv_imgcodecs\
-lopencv_videoio\
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
    View/TrackerParameterView.cpp \
    fish/history/History.cpp \
    fish/pose/FishPose.cpp \
    fish/Fish.cpp \
    fish/NN2dMapper.cpp \
    helper/CvHelper.cpp \
    helper/StringHelper.cpp \
    imageProcessor/cvblobs/blob.cpp \
    imageProcessor/cvblobs/BlobContour.cpp \
    imageProcessor/cvblobs/BlobOperators.cpp \
    imageProcessor/cvblobs/BlobProperties.cpp \
    imageProcessor/cvblobs/BlobResult.cpp \
    imageProcessor/cvblobs/ComponentLabeling.cpp \
    imageProcessor/detector/blob/cvBlob/BlobsDetector.cpp \
    imageProcessor/detector/blob/simpleBlob/SimpleBlobsDetector.cpp \
    imageProcessor/detector/blob/BlobPose.cpp \
    imageProcessor/detector/contour/ContourPose.cpp \
    imageProcessor/detector/contour/ContoursDetector.cpp \
    imageProcessor/detector/IDetector.cpp \
    imageProcessor/preprocessor/ImagePreProcessor.cpp \
    property/IParam.cpp \
    property/Param.cpp \
    property/Rectification.cpp \
    property/SystemProperty.cpp \
    PluginContext.cpp


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
    View/TrackerParameterView.h \
    fish/history/History.h \
    fish/pose/FishPose.h \
    fish/pose/IPose.h \
    fish/Fish.h \
    fish/NN2dMapper.h \
    helper/CvHelper.h \
    helper/StringHelper.h \
    imageProcessor/cvblobs/blob.h \
    imageProcessor/cvblobs/BlobContour.h \
    imageProcessor/cvblobs/BlobLibraryConfiguration.h \
    imageProcessor/cvblobs/BlobOperators.h \
    imageProcessor/cvblobs/BlobProperties.h \
    imageProcessor/cvblobs/BlobResult.h \
    imageProcessor/cvblobs/ComponentLabeling.h \
    imageProcessor/detector/blob/cvBlob/BlobsDetector.h \
    imageProcessor/detector/blob/simpleBlob/SimpleBlobsDetector.h \
    imageProcessor/detector/blob/BlobPose.h \
    imageProcessor/detector/contour/ContourPose.h \
    imageProcessor/detector/contour/ContoursDetector.h \
    imageProcessor/detector/IDetector.h \
    imageProcessor/preprocessor/ImagePreProcessor.h \
    property/IParam.h \
    property/Messages.h \
    property/Param.h \
    property/ParamNames.h \
    property/Rectification.h \
    property/SystemProperty.h \
    PluginContext.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    BioTrackerPlugin.json \
    CMakeLists.txt

FORMS += \
    View/TrackerParameterView.ui
