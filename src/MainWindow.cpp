#include "../MainWindow.h"

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

void MainWindow::frameCalculated(const size_t frameNumber,
                                 const std::string filename, const double currentFps) {
    setWindowTitle("BioTracker [" + QString::fromStdString(stem_filename(filename)) + "]");
}

} // Gui
} // BioTracker
