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
    return new TrackedTrajectory();
}
