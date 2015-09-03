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

void NotificationWidget::notify(const std::string &message, const MSGS::MTYPE type) {
    m_ui.edit_notification->append(QString(message.c_str()));
}

}
}
