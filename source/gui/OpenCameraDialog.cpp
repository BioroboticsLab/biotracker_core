#include "OpenCameraDialog.h"

namespace BioTracker {
namespace Gui {

OpenCameraDialog::OpenCameraDialog(QWidget *parent, Core::Facade &facade)
    : QDialog(parent)
	, m_ui(this)
    , m_facade(facade)
{

}

void OpenCameraDialog::initConnects()
{

}

}
}
