#ifndef ITRACKEDCOMPONENT_H
#define ITRACKEDCOMPONENT_H

#include "IModel.h"
/**
 * This is the common interface for all TrackedComponents.
 * It is part of the Composite Pattern and translates to the abstract Component class as stated by the GoF.
 * A TrackedComponent can have a QObject as parent and it
 * has a methode called operate() that needs to be implemented
 * by all derivatives of this interface class.
 */
class IModelTrackedComponent : public IModel
{
    Q_OBJECT

  public:
    /**
     * The constructor of the IModelTrackedComponent class is able to receive a QObject as parent.
     */
    IModelTrackedComponent(QObject *parent = 0);

    /**
     * The methode operate() must be implemented by every derivative of this class.
     */
    virtual void operate();
};

#endif // ITRACKEDCOMPONENT_H
