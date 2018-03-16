#include "TrackedTrajectory.h"
#include "QDebug"
#include "TrackedElement.h"

void TrackedTrajectory::triggerRecalcValid() {
    g_calcValid = 1;
}

void TrackedTrajectory::setValid(bool v) {
    _valid = v;
    if (_parentNode) {
        TrackedTrajectory* n = dynamic_cast<TrackedTrajectory*>(_parentNode);
        if (n)
            n->triggerRecalcValid();
    }
}

TrackedTrajectory::TrackedTrajectory(QObject *parent, QString name) :
	IModelTrackedTrajectory(parent),
	name(name)
{
	setFixed(false);
    g_calcValid = 1;
    g_validCount = 0;
    _size = 0;
    _valid = true;
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

    comp->setParent(this);

	if (pos < 0) {
		_TrackedComponents.append(comp);
        _size++;
        g_validCount++;
	}
	else if (_size <= pos) {
        while (_size < pos)
        {
            _TrackedComponents.append(nullptr);
            _size++;
        }

		_TrackedComponents.append(comp);
        _size++;
        g_validCount++;
	}
	else {
		_TrackedComponents[pos] = comp;
	}
}

bool TrackedTrajectory::remove(IModelTrackedComponent *comp)
{
    g_calcValid = 1;
    comp->setValid(false);
    return true;
}

void TrackedTrajectory::clear()
{
    g_calcValid = 1;
    foreach(IModelTrackedComponent* el, _TrackedComponents) {
        if (dynamic_cast<IModelTrackedTrajectory*>(el))
            dynamic_cast<IModelTrackedTrajectory*>(el)->clear();
    }
    _TrackedComponents.clear();
    _size = 0;
}

IModelTrackedComponent* TrackedTrajectory::getChild(int index)
{
    if (index < 0)
        return nullptr;

    if (index == _size - 1) {
        return _TrackedComponents.back();
    }

	return (_size > index ? _TrackedComponents.at(index) : nullptr);
}

IModelTrackedComponent* TrackedTrajectory::getValidChild(int index)
{
    int c = 0;
    foreach(IModelTrackedComponent* el, _TrackedComponents) {
        if (el){
            if (c == index && el->getValid())
                return el;
            c += el->getValid() ? 1 : 0;
        }
    }

    return nullptr;
}

IModelTrackedComponent* TrackedTrajectory::getLastChild()
{
    if (_TrackedComponents.empty())
        return nullptr;
	return _TrackedComponents.back();
}

int TrackedTrajectory::size()
{
    return (_size == 0 ? _size = _TrackedComponents.size() : _size);
}

int TrackedTrajectory::validCount()
{
    if (g_calcValid == 1) {
        int c = 0;
        foreach(IModelTrackedComponent* el, _TrackedComponents) {
            if (el)
                c += el->getValid() ? 1 : 0;
        }

        g_validCount = c;
        g_calcValid = 0;
        return c;
    }
    else {
        return g_validCount;
    }
}
