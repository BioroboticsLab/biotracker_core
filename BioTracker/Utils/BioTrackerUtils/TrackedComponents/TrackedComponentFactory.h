#ifndef TRACKEDCOMPONENTFACTORY_H
#define TRACKEDCOMPONENTFACTORY_H

#include "Interfaces/IModel/IModelTrackedComponentFactory.h"

class TrackedComponentFactory : public IModelTrackedComponentFactory
{
    Q_OBJECT
public:
    TrackedComponentFactory();
    ~TrackedComponentFactory();

    // ITrackedComponentFactory interface
protected:
    IModelTrackedComponent *createTrackedElement() override;
    IModelTrackedComponent *createTrackedObject() override;
    IModelTrackedComponent *createTrackedTrajectory() override;
};

#endif // TRACKEDOBJECTFACTORY_H
