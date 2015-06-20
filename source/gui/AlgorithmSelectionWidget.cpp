#include "AlgorithmSelectionWidget.h"

namespace BioTracker {
namespace Gui {

AlgorithmSelectionWidget::AlgorithmSelectionWidget(QWidget *parent, Core::Facade &facade)
    : QWidget(parent)
    , m_ui(parent)
    , m_facade(facade)
{
}

void AlgorithmSelectionWidget::initConnects()
{

}

}
}
