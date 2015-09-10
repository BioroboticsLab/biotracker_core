#include "AlgorithmSelectionWidget.h"

#include <QString>

#include "biotracker/core/Registry.h"

namespace BioTracker {
namespace Gui {

AlgorithmSelectionWidget::AlgorithmSelectionWidget(QWidget *parent,
        Core::BioTrackerApp &facade)
    : QWidget(parent)
    , m_ui(parent)
    , m_facade(facade) {
    initConnects();
    initAlgorithmList();

    m_ui.cb_algorithms->setInsertPolicy(
        QComboBox::InsertPolicy::InsertAlphabetically);
}

void AlgorithmSelectionWidget::addTrackingAlgorithm(const Core::TrackerType
        type) {
    m_ui.cb_algorithms->addItem(QString::fromStdString(
                                    m_facade.getRegistry().getStringByType().at(type)));
}

void AlgorithmSelectionWidget::initAlgorithmList() {
    // add NoTracker first
    m_ui.cb_algorithms->addItem(QString::fromStdString(
                                    m_facade.getRegistry().getStringByType().at(Core::NoTracking)));

    // add Trackers
    for (auto &algByStr : m_facade.getRegistry().getTypeByString()) {
        if (algByStr.second != Core::NoTracking) {
            m_ui.cb_algorithms->addItem(QString::fromStdString(algByStr.first));
        }
    }
}

void AlgorithmSelectionWidget::initConnects() {
    QObject::connect(&m_facade.getRegistry(), &Core::Registry::newTracker,
                     this, &AlgorithmSelectionWidget::addTrackingAlgorithm);
}

}
}
