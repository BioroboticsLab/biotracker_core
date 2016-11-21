#ifndef ITRACKEDCOMPONENTFACTORY_H
#define ITRACKEDCOMPONENTFACTORY_H

#include "Interfaces/IModel/imodel.h"
#include "Interfaces/IModel/ITrackedComponent.h"

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
