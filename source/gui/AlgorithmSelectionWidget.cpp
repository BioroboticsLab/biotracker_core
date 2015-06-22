#include "AlgorithmSelectionWidget.h"

#include <QString>

#include "source/core/Registry.h"

namespace BioTracker {
namespace Gui {

AlgorithmSelectionWidget::AlgorithmSelectionWidget(QWidget *parent, Core::Facade &facade)
    : QWidget(parent)
    , m_ui(parent)
    , m_facade(facade)
{
    initConnects();
    initAlgorithmList();

    m_ui.cb_algorithms->setInsertPolicy(QComboBox::InsertPolicy::InsertAlphabetically);
}

void AlgorithmSelectionWidget::addTrackingAlgorithm(const Core::TrackerType type)
{
    m_ui.cb_algorithms->addItem(QString::fromStdString(m_facade.getRegistry().stringByType().at(type)));
}

void AlgorithmSelectionWidget::initAlgorithmList()
{
    // add NoTracker first
    m_ui.cb_algorithms->addItem(QString::fromStdString(
        m_facade.getRegistry().stringByType().at(Core::NoTracking)));

    // add Trackers
    for (auto& algByStr : m_facade.getRegistry().typeByString())
    {
        if (algByStr.second != Core::NoTracking)
        {
            m_ui.cb_algorithms->addItem(QString::fromStdString(algByStr.first));
        }
    }
}

void AlgorithmSelectionWidget::initConnects()
{
    QObject::connect(&m_facade.getRegistry(), &Core::Registry::newTracker,
                     this, &AlgorithmSelectionWidget::addTrackingAlgorithm);
}

}
}
