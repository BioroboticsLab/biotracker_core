#pragma once

#include <QWidget>

#include "source/core/Facade.h"
#include "source/gui/ui_NotificationWidget.h"
#include "source/util/QtRaiiWrapper.hpp"

namespace BioTracker {
namespace Gui {

class NotificationWidget : public QWidget
{
    Q_OBJECT
public:
    typedef Util::QtRaiiWrapper<Ui::NotificationWidget, QWidget> NotificationUi;

    explicit NotificationWidget(QWidget *parent, Core::Facade &facade);

    NotificationUi& getUi() { return m_ui; }

private:
    NotificationUi m_ui;
    Core::Facade& m_facade;

    void initConnects();

private slots:
};

}
}
