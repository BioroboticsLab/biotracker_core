#pragma once

#include <QWidget>

#include "biotracker/core/BioTrackerApp.h"
#include "biotracker/gui/ui_AlgorithmSelectionWidget.h"
#include "biotracker/util/QtRaiiWrapper.hpp"

class Facade;

namespace BioTracker {
namespace Gui {

class AlgorithmSelectionWidget : public QWidget {
    Q_OBJECT
  public:
    typedef Util::QtRaiiWrapper<Ui::AlgorithmSelectionWidget, QWidget>
    AlgorithmSelectionUi;

    explicit AlgorithmSelectionWidget(QWidget *parent, Core::BioTrackerApp &facade);

    AlgorithmSelectionUi &getUi() {
        return m_ui;
    }

  public Q_SLOTS:
    void addTrackingAlgorithm(const Core::TrackerType type);

  private:
    AlgorithmSelectionUi m_ui;
    Core::BioTrackerApp &m_facade;

    void initAlgorithmList();
    void initConnects();
};

}
}

