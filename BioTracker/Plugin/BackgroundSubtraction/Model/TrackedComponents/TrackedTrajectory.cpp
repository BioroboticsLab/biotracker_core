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

void TrackedTrajectory::add(IModelTrackedComponent *comp, int pos)
{
	if (pos < 0) {
		m_TrackedComponents.append(comp);
	}
	else if (m_TrackedComponents.size() <= pos) {
		while (m_TrackedComponents.size() < pos)
			m_TrackedComponents.append(0);

		m_TrackedComponents.append(comp);
	}
	else {
		m_TrackedComponents[pos] = comp;
	}
}

bool TrackedTrajectory::remove(IModelTrackedComponent *comp)
{
	return m_TrackedComponents.removeOne(comp); //Do not actually remove, just invalidate / replace by dummy
}

void TrackedTrajectory::clear()
{
	m_TrackedComponents.clear();
}

IModelTrackedComponent* TrackedTrajectory::getChild(int index)
{
	return (m_TrackedComponents.size() > index ? m_TrackedComponents.at(index) : nullptr);
}

IModelTrackedComponent* TrackedTrajectory::getLastChild()
{
	return m_TrackedComponents.at(m_TrackedComponents.size()-1);
}

int TrackedTrajectory::size()
{
    return m_TrackedComponents.size();
}

int TrackedTrajectory::validCount()
{
    int c = 0;
    foreach(IModelTrackedComponent* el, m_TrackedComponents){
        if (el)
            c += el->getValid() ? 1 : 0;
    }

    return c;
}
