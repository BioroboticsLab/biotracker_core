#pragma once

#include <QWidget>

#include "biotracker/BioTrackerApp.h"
#include "ui_AlgorithmSelectionWidget.h"
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
    Core::BioTrackerApp &m_biotracker;

    void initAlgorithmList();
    void initConnects();

  private Q_SLOTS:
    void trackingAlgorithmSelected(const QString& name);
};

}
}

