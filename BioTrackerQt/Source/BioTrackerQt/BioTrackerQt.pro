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


TARGET = BioTrackerQt
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


SOURCES += main.cpp\
    BioTracker3App.cpp \
    Interfaces/IBioTrackerContext.cpp \
    GuiContext.cpp \
    Controller/ControllerContext.cpp \
    Controller/ControllerGraphicScene.cpp \
    Controller/ControllerMainWindow.cpp \
    Controller/ControllerPlayer.cpp \
    Controller/ControllerTextureObject.cpp \
    Controller/ControllerTrackedComponent.cpp \
    Controller/ControllerTrackingAlgorithm.cpp \
    Interfaces/IController/icontroller.cpp \
    Interfaces/IController/IControllerStrategy.cpp \
    Interfaces/IModel/imodel.cpp \
    Interfaces/IModel/IObject.cpp \
    Interfaces/IModel/ITrackedComponent.cpp \
    Interfaces/IModel/ITrackedComponentFactory.cpp \
    Interfaces/IModel/ITrackedTrajectory.cpp \
    Interfaces/IModel/ITrackingAlgorithm.cpp \
    Interfaces/IStates/IPlayerState.cpp \
    Interfaces/IView/IGraphicsPixmapItem.cpp \
    Interfaces/IView/IGraphicsView.cpp \
    Interfaces/IView/IView.cpp \
    Interfaces/IView/IViewGraphicsScene.cpp \
    Interfaces/IView/IViewMainWindow.cpp \
    Interfaces/IView/IViewOpenGLWidget.cpp \
    Interfaces/IView/IViewTrackedComponent.cpp \
    Interfaces/IView/IViewWidget.cpp \
    Model/PlayerStates/PStateInitial.cpp \
    Model/PlayerStates/PStateInitialStream.cpp \
    Model/PlayerStates/PStatePause.cpp \
    Model/PlayerStates/PStatePlay.cpp \
    Model/PlayerStates/PStateStepBack.cpp \
    Model/PlayerStates/PStateStepForw.cpp \
    Model/PlayerStates/PStateWait.cpp \
    Model/TrackedComponents/TrackedComponentFactory.cpp \
    Model/TrackedComponents/TrackedElement.cpp \
    Model/TrackedComponents/TrackedTrajectory.cpp \
    Model/BioTracker3ImageStream.cpp \
    Model/BioTracker3Player.cpp \
    Model/BioTracker3ProxyMat.cpp \
    Model/BioTracker3TextureObject.cpp \
    Model/BioTrackerTrackingAlgorithm.cpp \
    Model/null_Model.cpp \
    View/BioTracker3MainWindow.cpp \
    View/BioTracker3VideoControllWidget.cpp \
    View/BioTracker3VideoView.cpp \
    View/GraphicsScene.cpp \
    View/GraphicsView.cpp \
    View/TextureObjectView.cpp \
    View/TrackedElementView.cpp \
    View/TrackedImageView.cpp \
    src/settings/Settings.cpp \
    src/settings/StringTranslator.cpp \
    src/settings/SystemCompatibilityCheck.cpp \
    src/util/imagePreProcessor/ImagePreProcessor.cpp \
    src/util/CvHelper.cpp \
    src/util/stringTools.cpp \
    Model/PluginLoader.cpp \
    View/CameraDeviceSelect.cpp \
    View/CameraDevice.cpp \
    Controller/null_Controller.cpp

HEADERS  += BioTracker3App.h \
    Interfaces/IBioTrackerContext.h \
    GuiContext.h \
    Controller/ControllerContext.h \
    Controller/ControllerGraphicScene.h \
    Controller/ControllerMainWindow.h \
    Controller/ControllerPlayer.h \
    Controller/ControllerTextureObject.h \
    Controller/ControllerTrackedComponent.h \
    Controller/ControllerTrackingAlgorithm.h \
    Interfaces/IController/icontroller.h \
    Interfaces/IController/IControllerStrategy.h \
    Interfaces/IModel/imodel.h \
    Interfaces/IModel/IObject.h \
    Interfaces/IModel/ITrackedComponent.h \
    Interfaces/IModel/ITrackedComponentFactory.h \
    Interfaces/IModel/ITrackedTrajectory.h \
    Interfaces/IModel/ITrackingAlgorithm.h \
    Interfaces/IStates/IPlayerState.h \
    Interfaces/IView/IGraphicsPixmapItem.h \
    Interfaces/IView/IGraphicsView.h \
    Interfaces/IView/IView.h \
    Interfaces/IView/IViewGraphicsScene.h \
    Interfaces/IView/IViewMainWindow.h \
    Interfaces/IView/IViewOpenGLWidget.h \
    Interfaces/IView/IViewTrackedComponent.h \
    Interfaces/IView/IViewWidget.h \
    Interfaces/ENUMS.h \
    Model/PlayerStates/PStateInitial.h \
    Model/PlayerStates/PStateInitialStream.h \
    Model/PlayerStates/PStatePause.h \
    Model/PlayerStates/PStatePlay.h \
    Model/PlayerStates/PStateStepBack.h \
    Model/PlayerStates/PStateStepForw.h \
    Model/PlayerStates/PStateWait.h \
    Model/TrackedComponents/TrackedComponentFactory.h \
    Model/TrackedComponents/TrackedElement.h \
    Model/TrackedComponents/TrackedTrajectory.h \
    Model/BioTracker3ImageStream.h \
    Model/BioTracker3Player.h \
    Model/BioTracker3ProxyMat.h \
    Model/BioTracker3TextureObject.h \
    Model/BioTrackerTrackingAlgorithm.h \
    Model/null_Model.h \
    View/BioTracker3MainWindow.h \
    View/BioTracker3VideoControllWidget.h \
    View/BioTracker3VideoView.h \
    View/GraphicsScene.h \
    View/GraphicsView.h \
    View/TextureObjectView.h \
    View/TrackedElementView.h \
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
    Exceptions.h \
    PanZoomState.h \
    Model/PluginLoader.h \
    View/CameraDeviceSelect.h \
    View/CameraDevice.h \
    Controller/null_Controller.h

FORMS    += View/BioTracker3MainWindow.ui \
    View/BioTracker3VideoControllWidget.ui \
    View/CameraDeviceSelect.ui \
    View/CameraDevice.ui

RESOURCES += \
    guiresources.qrc

