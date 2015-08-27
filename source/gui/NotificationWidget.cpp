#include "NotificationWidget.h"

namespace BioTracker {
namespace Gui {

NotificationWidget::NotificationWidget(QWidget *parent,
                                       Core::BioTrackerApp &facade)
    : QWidget(parent)
    , m_ui(parent)
    , m_facade(facade) {

}

void NotificationWidget::initConnects() {

}

}
}
