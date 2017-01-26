#ifndef ITRACKEDOTRAJECTORY_H
#define ITRACKEDOTRAJECTORY_H

#include "Interfaces/IModel/IModelTrackedComponent.h"

/**
 * This interface class derives from IModelTrackedComponent.
 * Its purpose is to define the interface for a Composite class that takes the responsibility
 * for adding, deleting and returning child objects.
 * This class is part of the Composite Pattern and translates to the abstract Composite class
 * as stated by the GoF.
 *
 * Its the Plugin Developers full responsibility to choos an internal data structure. This could be e.g. Map, List or Vector.
 */
class IModelTrackedTrajectory : public IModelTrackedComponent
{
    Q_OBJECT

public:
    /**
     * The constructor.
     */
    IModelTrackedTrajectory(QObject *parent);

    /**
     * This methode must be implemented by all derivatives. Its purpose is to provide a mechanism for adding Leaf components to this structure.
     */
    virtual void add(IModelTrackedComponent *comp) = 0;

    /**
     * This methode must be implemented by all derivatives. Its functionality shall be a mechanism for removing Leaf components from this structure.
     */
    virtual void remove(IModelTrackedComponent *comp) = 0;

    /**
     * This shloud simply returns a child object by an index.
     */
    virtual IModelTrackedComponent* getChild(int index)  = 0;

    // ITrackedComponent interface
public:
    void operate();
};

#endif // ITRACKEDOTRAJECTORY_H
