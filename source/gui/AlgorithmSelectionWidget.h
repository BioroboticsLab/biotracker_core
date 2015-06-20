#pragma once

#include <QWidget>

#include "source/core/Facade.h"
#include "source/gui/ui_AlgorithmSelectionWidget.h"
#include "source/util/QtRaiiWrapper.hpp"

namespace BioTracker {
namespace Gui {

class AlgorithmSelectionWidget : public QWidget
{
    Q_OBJECT
public:
    typedef Util::QtRaiiWrapper<Ui::AlgorithmSelectionWidget, QWidget> AlgorithmSelectionUi;

    explicit AlgorithmSelectionWidget(QWidget *parent, Core::Facade &facade);

    AlgorithmSelectionUi& getUi() { return m_ui; }

private:
    AlgorithmSelectionUi m_ui;
    Core::Facade& m_facade;

    void initConnects();

private slots:
};

}
}

