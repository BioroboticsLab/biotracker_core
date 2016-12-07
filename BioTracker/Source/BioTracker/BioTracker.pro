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

LIBS += -L/usr/include/boost -lboost_system
LIBS += -L/usr/include/boost  -lboost_chrono
LIBS += -L/usr/include/boost  -lboost_thread
LIBS += -L/usr/include/boost  -lboost_timer
LIBS += -L/usr/include/boost  -lboost_filesystem

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

#INCLUDEPATH += ../../Interfaces/BioTrackerInterfaces/
#LIBS += /home/andreas/Documents/biotracker_core/BioTracker/Interfaces/build-BioTrackerInterfaces-BioTracker-Debug/libBioTrackerInterfaces.a



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
    src/settings/Settings.cpp \
    src/settings/StringTranslator.cpp \
    src/settings/SystemCompatibilityCheck.cpp \
    src/util/imagePreProcessor/ImagePreProcessor.cpp \
    src/util/CvHelper.cpp \
    src/util/stringTools.cpp \
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
    settings/Exceptions.h \
    settings/PanZoomState.h \
    View/TextureObjectView.h

FORMS    += \
    View/CameraDevice.ui \
    View/MainWindow.ui \
    View/VideoControllWidget.ui

RESOURCES += \
    guiresources.qrc




win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Interfaces/build-BioTrackerInterfaces-BioTracker-Debug/release/ -lBioTrackerInterfaces
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Interfaces/build-BioTrackerInterfaces-BioTracker-Debug/debug/ -lBioTrackerInterfaces
else:unix: LIBS += -L$$PWD/../../Interfaces/build-BioTrackerInterfaces-BioTracker-Debug/ -lBioTrackerInterfaces

INCLUDEPATH += $$PWD/../../Interfaces/BioTrackerInterfaces
DEPENDPATH += $$PWD/../../Interfaces/BioTrackerInterfaces

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../Interfaces/build-BioTrackerInterfaces-BioTracker-Debug/release/libBioTrackerInterfaces.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../Interfaces/build-BioTrackerInterfaces-BioTracker-Debug/debug/libBioTrackerInterfaces.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../Interfaces/build-BioTrackerInterfaces-BioTracker-Debug/release/BioTrackerInterfaces.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../Interfaces/build-BioTrackerInterfaces-BioTracker-Debug/debug/BioTrackerInterfaces.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../Interfaces/build-BioTrackerInterfaces-BioTracker-Debug/libBioTrackerInterfaces.a
