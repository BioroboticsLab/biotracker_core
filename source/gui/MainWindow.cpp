#include "MainWindow.h"

namespace BioTracker {
namespace Gui {

MainWindow::MainWindow(Core::Facade &facade)
    : m_algorithmSelection(this, facade)
    , m_notification(this, facade)
    , m_videoControl(this, facade, m_ui.videoView)
{
    m_ui.setupUi(this);
    m_algorithmSelection.getUi().setupUi(m_ui.widget_alg);
    m_notification.getUi().setupUi(m_ui.dockWidgetNotificationContents);
    m_videoControl.getUi().setupUi(m_ui.videoControls);
}

}
}
