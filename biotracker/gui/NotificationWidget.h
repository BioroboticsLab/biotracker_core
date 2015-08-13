#pragma once

#include <QWidget>

#include "biotracker/core/BioTrackerApp.h"
#include "biotracker/gui/ui_NotificationWidget.h"
#include "biotracker/util/QtRaiiWrapper.hpp"

namespace BioTracker {
namespace Gui {

class NotificationWidget : public QWidget {
    Q_OBJECT
  public:
    typedef Util::QtRaiiWrapper<Ui::NotificationWidget, QWidget> NotificationUi;

    explicit NotificationWidget(QWidget *parent, Core::BioTrackerApp &facade);

    NotificationUi &getUi() {
        return m_ui;
    }

  private:
    NotificationUi m_ui;
    Core::BioTrackerApp &m_facade;

    void initConnects();

  private slots:
};

}
}
