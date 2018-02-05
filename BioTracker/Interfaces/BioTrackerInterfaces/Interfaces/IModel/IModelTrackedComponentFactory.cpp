#include "IModelTrackedComponentFactory.h"

IModelTrackedComponentFactory::IModelTrackedComponentFactory() {

}

IModelTrackedComponentFactory::~IModelTrackedComponentFactory()
{

}

IModelTrackedComponent *IModelTrackedComponentFactory::getNewTrackedElement(QString name)
{
    return createTrackedElement(name);
}

IModelTrackedComponent *IModelTrackedComponentFactory::getNewTrackedObject(QString name)
{
    return createTrackedObject(name);
}

IModelTrackedComponent *IModelTrackedComponentFactory::getNewTrackedTrajectory(QString name)
{
	return createTrackedTrajectory(name);
}
