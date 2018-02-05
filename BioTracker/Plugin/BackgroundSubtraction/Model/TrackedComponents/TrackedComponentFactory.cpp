#include "TrackedComponentFactory.h"

#include "TrackedTrajectory.h"
#include "TrackedElement.h"

TrackedComponentFactory::TrackedComponentFactory()
{

}

TrackedComponentFactory::~TrackedComponentFactory()
{

}

QList<QString> TrackedComponentFactory::getElementTypes() {
    return QList<QString>{ "TrackedElement" };
}

IModelTrackedComponent *TrackedComponentFactory::createTrackedElement(QString name)
{
    return new TrackedElement(this, "n.a.");
}

IModelTrackedComponent *TrackedComponentFactory::createTrackedObject(QString name)
{
	TrackedTrajectory *t	= new TrackedTrajectory();
	TrackedElement *e		= new TrackedElement(this, "n.a.", 0);
	t->add(e, 0);
	return t;
}

IModelTrackedComponent *TrackedComponentFactory::createTrackedTrajectory(QString name)
{
	return new TrackedTrajectory();
}
