#ifndef ITRACKEDOTRAJECTORY_H
#define ITRACKEDOTRAJECTORY_H

#include "Interfaces/IModel/IModelTrackedComponent.h"

class IModelTrackedTrajectory : public IModelTrackedComponent
{
    Q_OBJECT

public:
    IModelTrackedTrajectory(QObject *parent);

    virtual void add(IModelTrackedComponent *comp) = 0;
    virtual void remove(IModelTrackedComponent *comp) = 0;
    virtual IModelTrackedComponent* getChild(int index)  = 0;

    // ITrackedComponent interface
public:
    void operate();
};

#endif // ITRACKEDOTRAJECTORY_H
