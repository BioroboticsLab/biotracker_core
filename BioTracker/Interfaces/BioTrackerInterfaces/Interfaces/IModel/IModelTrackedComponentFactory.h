#ifndef ITRACKEDCOMPONENTFACTORY_H
#define ITRACKEDCOMPONENTFACTORY_H

#include "Interfaces/IModel/IModel.h"
#include "Interfaces/IModel/IModelTrackedComponent.h"

class IModelTrackedComponentFactory : public IModel
{
    Q_OBJECT

  public:
    IModelTrackedComponentFactory();
    virtual ~IModelTrackedComponentFactory() = 0;

    IModelTrackedComponent *getNewTrackedElement();
    IModelTrackedComponent *getNewTrackedObject();

protected:
    virtual IModelTrackedComponent *createTrackedElement() = 0;
    virtual IModelTrackedComponent *createTrackedObject() = 0;

};

#endif // ITRACKEDCOMPONENTFACTORY_H
