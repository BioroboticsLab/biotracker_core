#include "ITrackedComponentFactory.h"

ITrackedComponentFactory::ITrackedComponentFactory() {

}

ITrackedComponentFactory::~ITrackedComponentFactory()
{

}

ITrackedComponent *ITrackedComponentFactory::getNewTrackedElement()
{
    return createTrackedElement();
}

ITrackedComponent *ITrackedComponentFactory::getNewTrackedObject()
{
    return createTrackedObject();
}
