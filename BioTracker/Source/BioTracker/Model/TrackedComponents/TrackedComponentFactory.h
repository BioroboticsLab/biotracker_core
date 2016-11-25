#ifndef TRACKEDCOMPONENTFACTORY_H
#define TRACKEDCOMPONENTFACTORY_H

#include "Interfaces/IModel/ITrackedComponentFactory.h"

class TrackedComponentFactory : public ITrackedComponentFactory
{
    Q_OBJECT
public:
    TrackedComponentFactory();
    ~TrackedComponentFactory();

    // ITrackedComponentFactory interface
protected:
    ITrackedComponent *createTrackedElement() override;
    ITrackedComponent *createTrackedObject() override;
};

#endif // TRACKEDOBJECTFACTORY_H
