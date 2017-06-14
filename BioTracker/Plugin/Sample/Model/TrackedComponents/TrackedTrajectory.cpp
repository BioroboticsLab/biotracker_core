#include "TrackedTrajectory.h"
#include "QDebug"
#include "TrackedElement.h"

TrackedTrajectory::TrackedTrajectory(QObject *parent, QString name) :
    IModelTrackedTrajectory(parent),
    name(name)
{

}

void TrackedTrajectory::operate()
{
    qDebug() << "Printing all TrackedElements in TrackedObject " <<  name;
    qDebug() << "========================= Begin ==========================";
    for (int i = 0; i < m_TrackedComponents.size(); ++i) {
        dynamic_cast<TrackedElement *>(m_TrackedComponents.at(i))->operate();
    }
    qDebug() << "========================   End   =========================";
}

void TrackedTrajectory::add(IModelTrackedComponent *comp)
{
    m_TrackedComponents.append(comp);
}

bool TrackedTrajectory::remove(IModelTrackedComponent *comp)
{
	return m_TrackedComponents.removeOne(comp);
}

void TrackedTrajectory::clear()
{
	m_TrackedComponents.clear();
}

IModelTrackedComponent* TrackedTrajectory::getChild(int index)
{
	return m_TrackedComponents.at(index);
}

IModelTrackedComponent* TrackedTrajectory::getLastChild()
{
	return m_TrackedComponents.at(m_TrackedComponents.size()-1);
}

int TrackedTrajectory::size()
{
    return m_TrackedComponents.size();
}
