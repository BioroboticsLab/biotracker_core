#ifndef ITRACKEDCOMPONENT_H
#define ITRACKEDCOMPONENT_H

#include "IModel.h"

class IModelTrackedComponent : public IModel
{
    Q_OBJECT

  public:
    IModelTrackedComponent(QObject *parent = 0);

    virtual void operate();
};

#endif // ITRACKEDCOMPONENT_H
