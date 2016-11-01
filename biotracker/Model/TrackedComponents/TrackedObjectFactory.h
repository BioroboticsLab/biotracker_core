#ifndef TRACKEDOBJECTFACTORY_H
#define TRACKEDOBJECTFACTORY_H

#include "Interfaces/ITrackedComponentFactory.h"

class TrackedObjectFactory : public ITrackedComponentFactory
{
    Q_OBJECT
public:
    TrackedObjectFactory();
    ~TrackedObjectFactory();

    // ITrackedComponentFactory interface
protected:
    ITrackedComponent *createTrackedElement() override;
    ITrackedComponent *createTrackedObject() override;
};

#endif // TRACKEDOBJECTFACTORY_H
