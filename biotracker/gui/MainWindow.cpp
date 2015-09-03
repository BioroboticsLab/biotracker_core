#include "MainWindow.h"

namespace BioTracker {
namespace Gui {

MainWindow::MainWindow(Core::BioTrackerApp &biotracker)
    : m_ui(this)
    , m_algorithmSelection(m_ui.widget_alg, biotracker)
    , m_notification(m_ui.dockWidgetNotificationContents, biotracker)
    , m_openCameraDialog(m_ui.centralWidget, biotracker) {
    initalizeVideoView(biotracker);
}

void MainWindow::initalizeVideoView(Core::BioTrackerApp &biotracker) {
    m_videoView = std::make_unique<VideoView>(m_ui.trackingArea, biotracker);
    m_ui.videoViewLayout->addWidget(m_videoView.get());
    m_videoControl = std::make_unique<VideoControlWidget>(m_ui.videoControls,
                     biotracker, m_videoView.get());
}

void MainWindow::notify(const std::string &message, const MSGS::MTYPE type) {
    switch (type) {
    case MSGS::MTYPE::FILE_NAME_CHANGE: {
        QString filename = QString(message.c_str());
        statusBar()->showMessage(filename);
        setWindowTitle("BioTracker [" + filename + "]");
        break;
    }
    default: {
    }
    }
}

} // Gui
} // BioTracker
