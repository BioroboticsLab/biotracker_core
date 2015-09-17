#pragma once

#include <QMainWindow>

#include "biotracker/BioTrackerApp.h"
#include "ui_MainWindow.h"
#include "AlgorithmSelectionWidget.h"
#include "NotificationWidget.h"
#include "VideoControlWidget.h"
#include "OpenCameraDialog.h"
#include "VideoView.h"
#include "biotracker/util/QtRaiiWrapper.hpp"
#include "biotracker/util/QOpenGLContextWrapper.h"
#include "biotracker/util/stringTools.h"

namespace BioTracker {
namespace Gui {

class MainWindow : public QMainWindow {
    Q_OBJECT
  public:
    typedef Util::QtRaiiWrapper<Ui::MainWindow, QMainWindow> MainWindowUi;

    MainWindow(Core::BioTrackerApp &biotracker);

    const MainWindowUi &getUi() const {
        return m_ui;
    }

    VideoView *getVideoView() {
        return m_videoView.get();
    }
    AlgorithmSelectionWidget &getAlgorithmSelection() {
        return m_algorithmSelection;
    }
    NotificationWidget &getNotification() {
        return m_notification;
    }
    VideoControlWidget *getVideoControl() {
        return m_videoControl.get();
    }
    OpenCameraDialog &getOpenCameraDialog() {
        return m_openCameraDialog;
    }

  public Q_SLOTS:
    /**
     * @brief frameCalculated
     *      handles the showing of filenames in statusbar
     */
    void frameCalculated(const size_t frameNumber,
                    const std::string filename, const double currentFps);

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
