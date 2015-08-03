#include "MainWindow.h"

namespace BioTracker {
namespace Gui {

MainWindow::MainWindow(Core::Facade &facade)
    : m_context(this)
    , m_ui(this)
    , m_algorithmSelection(m_ui.widget_alg, facade)
    , m_notification(m_ui.dockWidgetNotificationContents, facade)
    // TODO!!
    , m_videoControl(m_ui.videoControls, facade, m_videoView)
    , m_videoView(m_ui.trackingArea, &m_context)
    , m_openCameraDialog(m_ui.centralWidget, facade)
{
    m_ui.videoViewLayout->addWidget(&m_videoView);
}

}
}
