#pragma once

#include <QWidget>

#include "source/core/Facade.h"
#include "source/gui/ui_AlgorithmSelectionWidget.h"

namespace BioTracker {
namespace Gui {

class AlgorithmSelectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlgorithmSelectionWidget(QWidget *parent, Core::Facade &facade);

    Ui::AlgorithmSelectionWidget& getUi() { return m_ui; }

private:
    Ui::AlgorithmSelectionWidget m_ui;
    Core::Facade& m_facade;

    void initConnects();

private slots:
};

}
}

