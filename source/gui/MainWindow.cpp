#include "MainWindow.h"

namespace BioTracker {
namespace Gui {

MainWindow::MainWindow(Core::Facade &facade)
    : m_videoControl(this, facade, m_ui.videoView)
{
    m_ui.setupUi(this);
    m_videoControl.getUi().setupUi(m_ui.videoControls);
}

}
}
