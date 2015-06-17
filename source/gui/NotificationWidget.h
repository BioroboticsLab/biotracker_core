#pragma once

#include <QWidget>

#include "source/core/Facade.h"
#include "source/gui/ui_NotificationWidget.h"

namespace BioTracker {
namespace Gui {

class NotificationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationWidget(QWidget *parent, Core::Facade &facade);

    Ui::NotificationWidget& getUi() { return m_ui; }

private:
    Ui::NotificationWidget m_ui;
    Core::Facade& m_facade;

    void initConnects();

private slots:
};

}
}
