#-------------------------------------------------
#
# Project created by QtCreator 2016-11-18T19:15:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += opengl
QT += multimedia
QT += multimediawidgets


TARGET = BioTracker
TEMPLATE = app

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


SOURCES += main.cpp\
    BioTracker3App.cpp \
    GuiContext.cpp \
    Controller/ControllerGraphicScene.cpp \
    Controller/ControllerMainWindow.cpp \
    Controller/ControllerPlayer.cpp \
    Controller/ControllerTextureObject.cpp \
    Model/PlayerStates/PStateInitial.cpp \
    Model/PlayerStates/PStateInitialStream.cpp \
    Model/PlayerStates/PStatePause.cpp \
    Model/PlayerStates/PStatePlay.cpp \
    Model/PlayerStates/PStateStepBack.cpp \
    Model/PlayerStates/PStateStepForw.cpp \
    Model/PlayerStates/PStateWait.cpp \
    Model/BioTracker3ProxyMat.cpp \
    Model/null_Model.cpp \
    View/GraphicsScene.cpp \
    View/GraphicsView.cpp \
    View/TrackedImageView.cpp \
    Model/PluginLoader.cpp \
    View/CameraDevice.cpp \
    Controller/null_Controller.cpp \
    Controller/ControllerPlugin.cpp \
    IStates/IPlayerState.cpp \
    View/MainWindow.cpp \
    View/VideoControllWidget.cpp \
    View/GLVideoView.cpp \
    Model/ImageStream.cpp \
    Model/MediaPlayer.cpp \
    Model/TextureObject.cpp \
    View/TextureObjectView.cpp

HEADERS  += BioTracker3App.h \
    GuiContext.h \
    Controller/ControllerGraphicScene.h \
    Controller/ControllerMainWindow.h \
    Controller/ControllerPlayer.h \
    Controller/ControllerTextureObject.h \
    Model/PlayerStates/PStateInitial.h \
    Model/PlayerStates/PStateInitialStream.h \
    Model/PlayerStates/PStatePause.h \
    Model/PlayerStates/PStatePlay.h \
    Model/PlayerStates/PStateStepBack.h \
    Model/PlayerStates/PStateStepForw.h \
    Model/PlayerStates/PStateWait.h \
    Model/BioTracker3ProxyMat.h \
    Model/null_Model.h \
    View/GraphicsScene.h \
    View/GraphicsView.h \
    View/TrackedImageView.h \
    Model/PluginLoader.h \
    View/CameraDevice.h \
    Controller/null_Controller.h \
    Controller/ControllerPlugin.h \
    IStates/IPlayerState.h \
    View/MainWindow.h \
    View/VideoControllWidget.h \
    View/GLVideoView.h \
    Model/ImageStream.h \
    Model/MediaPlayer.h \
    Model/TextureObject.h \
    View/TextureObjectView.h

FORMS    += \
    View/CameraDevice.ui \
    View/MainWindow.ui \
    View/VideoControllWidget.ui

RESOURCES += \
    guiresources.qrc



# Include the BioTrackerInterfaces Library
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Interfaces/build-BioTrackerInterfaces-Desktop-Debug/release/ -lBioTrackerInterfaces
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Interfaces/build-BioTrackerInterfaces-Desktop-Debug/debug/ -lBioTrackerInterfaces
else:unix: LIBS += -L$$PWD/../../Interfaces/build-BioTrackerInterfaces-Desktop-Debug/ -lBioTrackerInterfaces

INCLUDEPATH += $$PWD/../../Interfaces/BioTrackerInterfaces
DEPENDPATH += $$PWD/../../Interfaces/BioTrackerInterfaces

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../Interfaces/build-BioTrackerInterfaces-Desktop-Debug/release/libBioTrackerInterfaces.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../Interfaces/build-BioTrackerInterfaces-Desktop-Debug/debug/libBioTrackerInterfaces.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../Interfaces/build-BioTrackerInterfaces-Desktop-Debug/release/BioTrackerInterfaces.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../Interfaces/build-BioTrackerInterfaces-Desktop-Debug/debug/BioTrackerInterfaces.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../Interfaces/build-BioTrackerInterfaces-Desktop-Debug/libBioTrackerInterfaces.a


# Include the BioTrackerUtils Library

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Utils/build-BioTrackerUtils-Desktop-Debug/release/ -lBioTrackerUtils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Utils/build-BioTrackerUtils-Desktop-Debug/debug/ -lBioTrackerUtils
else:unix: LIBS += -L$$PWD/../../Utils/build-BioTrackerUtils-Desktop-Debug/ -lBioTrackerUtils

INCLUDEPATH += $$PWD/../../Utils/BioTrackerUtils
DEPENDPATH += $$PWD/../../Utils/BioTrackerUtils

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../Utils/build-BioTrackerUtils-Desktop-Debug/release/libBioTrackerUtils.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../Utils/build-BioTrackerUtils-Desktop-Debug/debug/libBioTrackerUtils.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../Utils/build-BioTrackerUtils-Desktop-Debug/release/BioTrackerUtils.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../Utils/build-BioTrackerUtils-Desktop-Debug/debug/BioTrackerUtils.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../Utils/build-BioTrackerUtils-Desktop-Debug/libBioTrackerUtils.a
