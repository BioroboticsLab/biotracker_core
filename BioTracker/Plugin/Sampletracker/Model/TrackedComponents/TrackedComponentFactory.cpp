#include "TrackedComponentFactory.h"

#include "TrackedTrajectory.h"
#include "TrackedElement.h"

TrackedComponentFactory::TrackedComponentFactory()
{

}

TrackedComponentFactory::~TrackedComponentFactory()
{

}

IModelTrackedComponent *TrackedComponentFactory::createTrackedElement()
{
    return new TrackedElement(this, "n.a.");
}

IModelTrackedComponent *TrackedComponentFactory::createTrackedObject()
{
	TrackedTrajectory *t	= new TrackedTrajectory();
	TrackedElement *e		= new TrackedElement(this, "n.a.", 0);
	t->add(e);
	return t;
}
