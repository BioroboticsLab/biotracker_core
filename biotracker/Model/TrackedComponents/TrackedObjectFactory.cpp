#include "TrackedObjectFactory.h"

#include "TrackedObject.h"
#include "TrackedElement.h"

TrackedObjectFactory::TrackedObjectFactory()
{

}

TrackedObjectFactory::~TrackedObjectFactory()
{

}

ITrackedComponent *TrackedObjectFactory::createTrackedElement()
{
    return new TrackedElement();
}

ITrackedComponent *TrackedObjectFactory::createTrackedObject()
{
    return new TrackedObject();
}
