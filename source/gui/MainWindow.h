#pragma once

#include <QMainWindow>

#include "source/core/BioTrackerApp.h"
#include "source/gui/ui_MainWindow.h"
#include "source/gui/AlgorithmSelectionWidget.h"
#include "source/gui/NotificationWidget.h"
#include "source/gui/VideoControlWidget.h"
#include "source/gui/OpenCameraDialog.h"
#include "source/gui/VideoView.h"
#include "source/util/QtRaiiWrapper.hpp"
#include "source/util/QOpenGLContextWrapper.h"

namespace BioTracker {
namespace Gui {

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    typedef Util::QtRaiiWrapper<Ui::MainWindow, QMainWindow> MainWindowUi;

    MainWindow(Core::BioTrackerApp &biotracker);

    const MainWindowUi& getUi() const { return m_ui; }

    VideoView* getVideoView() { return m_videoView.get(); }
    AlgorithmSelectionWidget& getAlgorithmSelection() { return m_algorithmSelection; }
    NotificationWidget& getNotification() { return m_notification; }
    VideoControlWidget* getVideoControl() { return m_videoControl.get(); }
	OpenCameraDialog& getOpenCameraDialog() { return m_openCameraDialog; }

private:
    MainWindowUi m_ui;
    AlgorithmSelectionWidget m_algorithmSelection;
    NotificationWidget m_notification;
    std::unique_ptr<VideoView> m_videoView;
    std::unique_ptr<VideoControlWidget> m_videoControl;
    OpenCameraDialog m_openCameraDialog;

    void initalizeVideoView(Core::BioTrackerApp &biotracker);
};



}
}
