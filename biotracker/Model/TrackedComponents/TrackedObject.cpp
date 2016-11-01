#include "TrackedObject.h"
#include "QDebug"
#include "TrackedElement.h"

TrackedObject::TrackedObject(QObject *parent, QString name) :
    ITrackedObject(parent),
    name(name)
{

}

void TrackedObject::operate()
{
    qDebug() << "Printing all TrackedElements in TrackedObject " <<  name;
    qDebug() << "========================= Begin ==========================";
    for (int i = 0; i < m_TrackedObject.size(); ++i) {
        dynamic_cast<TrackedElement *>(m_TrackedObject.at(i))->operate();
    }
    qDebug() << "========================   End   =========================";
}

void TrackedObject::add(ITrackedComponent *comp)
{
    m_TrackedObject.append(comp);
}

void TrackedObject::remove(ITrackedComponent *comp)
{
    m_TrackedObject.removeOne(comp);
}

ITrackedComponent* TrackedObject::getChild(int index)
{
    return m_TrackedObject.at(index);
}

int TrackedObject::numberOfChildrean()
{
    return m_TrackedObject.size();
}
