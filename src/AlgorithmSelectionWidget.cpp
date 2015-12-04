#include "../AlgorithmSelectionWidget.h"

#include <QString>
#include <QComboBox>

#include "biotracker/Registry.h"

namespace BioTracker {
namespace Gui {

AlgorithmSelectionWidget::AlgorithmSelectionWidget(QWidget *parent,
        Core::BioTrackerApp &facade)
    : QWidget(parent)
    , m_ui(parent)
    , m_biotracker(facade) {
    initConnects();
    initAlgorithmList();

    m_ui.cb_algorithms->setInsertPolicy(
        QComboBox::InsertPolicy::InsertAlphabetically);
}

void AlgorithmSelectionWidget::addTrackingAlgorithm(const Core::TrackerType
        type) {
    m_ui.cb_algorithms->addItem(QString::fromStdString(
                                    m_biotracker.getRegistry().getStringByType().at(type)));
}

void AlgorithmSelectionWidget::initAlgorithmList() {
    // add NoTracker first
    m_ui.cb_algorithms->addItem(QString::fromStdString(
                                    m_biotracker.getRegistry().getStringByType().at(Core::NoTracking)));

    // add Trackers
    for (auto &algByStr : m_biotracker.getRegistry().getTypeByString()) {
        if (algByStr.second != Core::NoTracking) {
            m_ui.cb_algorithms->addItem(QString::fromStdString(algByStr.first));
        }
    }
}

void AlgorithmSelectionWidget::initConnects() {
    QObject::connect(&m_biotracker.getRegistry(), &Core::Registry::newTracker,
                     this, &AlgorithmSelectionWidget::addTrackingAlgorithm);
    QObject::connect(m_ui.cb_algorithms, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
                     this, &AlgorithmSelectionWidget::trackingAlgorithmSelected);
}

void AlgorithmSelectionWidget::trackingAlgorithmSelected(const QString &name)
{
    m_biotracker.setTrackingAlgorithm(name.toStdString());
}

}
}
