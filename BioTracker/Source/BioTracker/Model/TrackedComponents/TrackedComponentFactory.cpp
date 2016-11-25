#include "TrackedComponentFactory.h"

#include "TrackedTrajectory.h"
#include "TrackedElement.h"

TrackedComponentFactory::TrackedComponentFactory()
{

}

TrackedComponentFactory::~TrackedComponentFactory()
{

}

ITrackedComponent *TrackedComponentFactory::createTrackedElement()
{
    return new TrackedElement(this, "n.a.");
}

ITrackedComponent *TrackedComponentFactory::createTrackedObject()
{
    return new TrackedTrajectory();
}
