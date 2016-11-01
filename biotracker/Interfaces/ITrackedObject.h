#ifndef ITRACKEDOBJECT_H
#define ITRACKEDOBJECT_H

#include "Interfaces/ITrackedComponent.h"

class ITrackedObject : public ITrackedComponent
{
    Q_OBJECT

public:
    ITrackedObject(QObject *parent);

    virtual void add(ITrackedComponent *comp) = 0;
    virtual void remove(ITrackedComponent *comp) = 0;
    virtual ITrackedComponent* getChild(int index)  = 0;

    // ITrackedComponent interface
public:
    void operate();
};

#endif // ITRACKEDOBJECT_H
