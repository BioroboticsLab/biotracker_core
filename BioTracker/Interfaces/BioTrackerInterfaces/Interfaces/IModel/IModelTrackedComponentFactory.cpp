#include "IModelTrackedComponentFactory.h"

IModelTrackedComponentFactory::IModelTrackedComponentFactory() {

}

IModelTrackedComponentFactory::~IModelTrackedComponentFactory()
{

}

IModelTrackedComponent *IModelTrackedComponentFactory::getNewTrackedElement()
{
    return createTrackedElement();
}

IModelTrackedComponent *IModelTrackedComponentFactory::getNewTrackedObject()
{
    return createTrackedObject();
}

IModelTrackedComponent *IModelTrackedComponentFactory::getNewTrackedTrajectory()
{
	return createTrackedTrajectory();
}
