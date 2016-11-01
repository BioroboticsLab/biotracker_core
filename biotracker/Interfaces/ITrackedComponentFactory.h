#ifndef ITRACKEDCOMPONENTFACTORY_H
#define ITRACKEDCOMPONENTFACTORY_H

#include "Interfaces/imodel.h"
#include "Interfaces/ITrackedComponent.h"

class ITrackedComponentFactory : public IModel
{
    Q_OBJECT

  public:
    ITrackedComponentFactory();
    virtual ~ITrackedComponentFactory() = 0;

    ITrackedComponent *getNewTrackedElement();
    ITrackedComponent *getNewTrackedObject();

protected:
    virtual ITrackedComponent *createTrackedElement() = 0;
    virtual ITrackedComponent *createTrackedObject() = 0;

};

#endif // ITRACKEDCOMPONENTFACTORY_H
