#include "TrackedTrajectory.h"
#include "QDebug"
#include "TrackedElement.h"

TrackedTrajectory::TrackedTrajectory(QObject *parent, QString name) :
	IModelTrackedTrajectory(parent),
	name(name)
{
	setFixed(false);
    setValid(true);
}

void TrackedTrajectory::operate()
{
	qDebug() << "Printing all TrackedElements in TrackedObject " <<  name;
	qDebug() << "========================= Begin ==========================";
	for (int i = 0; i < _TrackedComponents.size(); ++i) {
		dynamic_cast<TrackedElement *>(_TrackedComponents.at(i))->operate();
	}
	qDebug() << "========================   End   =========================";
}

void TrackedTrajectory::add(IModelTrackedComponent *comp, int pos)
{
	if (pos < 0) {
		_TrackedComponents.append(comp);
	}
	else if (_TrackedComponents.size() <= pos) {
		while (_TrackedComponents.size() < pos)
			_TrackedComponents.append(0);

		_TrackedComponents.append(comp);
	}
	else {
		_TrackedComponents[pos] = comp;
	}
}

bool TrackedTrajectory::remove(IModelTrackedComponent *comp)
{
	return _TrackedComponents.removeOne(comp); //Do not actually remove, just invalidate / replace by dummy
}

void TrackedTrajectory::clear()
{
    foreach(IModelTrackedComponent* el, _TrackedComponents) {
        if (dynamic_cast<IModelTrackedTrajectory*>(el))
            dynamic_cast<IModelTrackedTrajectory*>(el)->clear();
    }
    _TrackedComponents.clear();
}

IModelTrackedComponent* TrackedTrajectory::getChild(int index)
{
	return (_TrackedComponents.size() > index ? _TrackedComponents.at(index) : nullptr);
}

IModelTrackedComponent* TrackedTrajectory::getLastChild()
{
	return _TrackedComponents.at(_TrackedComponents.size()-1);
}

IModelTrackedComponent* TrackedTrajectory::getValidChild(int index)
{
    int c = 0;
    foreach(IModelTrackedComponent* el, _TrackedComponents) {
        if (el) {
            if (c == index && el->getValid())
                return el;
            c += el->getValid() ? 1 : 0;
        }
    }

    return nullptr;
}

int TrackedTrajectory::size()
{
    return _TrackedComponents.size();
}

int TrackedTrajectory::validCount()
{
    int c = 0;
    foreach(IModelTrackedComponent* el, _TrackedComponents){
        if (el)
            c += el->getValid() ? 1 : 0;
    }

    return c;
}
