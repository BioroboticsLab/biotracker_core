#pragma once

#include <QMainWindow>

#include "biotracker/core/BioTrackerApp.h"
#include "biotracker/gui/ui_MainWindow.h"
#include "biotracker/gui/AlgorithmSelectionWidget.h"
#include "biotracker/gui/NotificationWidget.h"
#include "biotracker/gui/VideoControlWidget.h"
#include "biotracker/gui/OpenCameraDialog.h"
#include "biotracker/gui/VideoView.h"
#include "biotracker/util/QtRaiiWrapper.hpp"
#include "biotracker/util/QOpenGLContextWrapper.h"

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
     * @brief notify
     *      Status messages for the user interface
     */
    void fileOpened(const std::string filename, const size_t numFrames);

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
