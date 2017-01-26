#-------------------------------------------------
#
# Project created by QtCreator 2016-12-08T10:23:47
#
#-------------------------------------------------

QT       += widgets
QT       += opengl

TARGET = BioTrackerUtils
TEMPLATE = lib
CONFIG += staticlib

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


SOURCES += \
    src/settings/Settings.cpp \
    src/settings/StringTranslator.cpp \
    src/settings/SystemCompatibilityCheck.cpp \
    src/util/imagePreProcessor/ImagePreProcessor.cpp \
    src/util/CvHelper.cpp \
    src/util/stringTools.cpp

HEADERS += \
    settings/Messages.h \
    settings/ParamNames.h \
    settings/Settings.h \
    settings/StringTranslator.h \
    settings/SystemCompatibilityCheck.h \
    util/imagePreProcessor/IImageProcessor.h \
    util/imagePreProcessor/ImagePreProcessor.h \
    util/CvHelper.h \
    util/MutexWrapper.h \
    util/platform.h \
    util/QtRaiiWrapper.hpp \
    util/ScreenHelper.h \
    util/singleton.h \
    util/stdext.h \
    util/stringTools.h \
    Exceptions.h \
    PanZoomState.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
