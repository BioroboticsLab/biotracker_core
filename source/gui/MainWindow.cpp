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

/* TODO: move to ctor */
void MainWindow::initalizeVideoView(Core::BioTrackerApp &biotracker) {
    m_videoView = std::make_unique<VideoView>(m_ui.trackingArea, biotracker);
    m_ui.videoViewLayout->addWidget(m_videoView.get());
    m_videoControl = std::make_unique<VideoControlWidget>(m_ui.videoControls,
                     biotracker, m_videoView.get());
}

}
}
