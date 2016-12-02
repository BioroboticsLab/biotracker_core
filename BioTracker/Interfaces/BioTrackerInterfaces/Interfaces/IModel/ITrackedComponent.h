#ifndef ITRACKEDCOMPONENT_H
#define ITRACKEDCOMPONENT_H

#include "imodel.h"

class ITrackedComponent : public IModel
{
    Q_OBJECT

  public:
    ITrackedComponent(QObject *parent = 0);

    virtual void operate();
};

#endif // ITRACKEDCOMPONENT_H
