#ifndef ITRACKEDOTRAJECTORY_H
#define ITRACKEDOTRAJECTORY_H

#include "Interfaces/IModel/ITrackedComponent.h"

class ITrackedTrajectory : public ITrackedComponent
{
    Q_OBJECT

public:
    ITrackedTrajectory(QObject *parent);

    virtual void add(ITrackedComponent *comp) = 0;
    virtual void remove(ITrackedComponent *comp) = 0;
    virtual ITrackedComponent* getChild(int index)  = 0;

    // ITrackedComponent interface
public:
    void operate();
};

#endif // ITRACKEDOTRAJECTORY_H
