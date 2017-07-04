#ifndef ITRACKEDCOMPONENT_H
#define ITRACKEDCOMPONENT_H

#include "IModel.h"
/**
 * This is the common interface for all TrackedComponents (IModelTrackedTrajectory and any leaf class).
 * It is part of the Composite Pattern and is the equivalent to the abstract Component class.
 * This class is derived from IModel from the Model-View-Controller structure.
 * A TrackedComponent has a QObject as parent and it
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

	virtual void  setX(float x) = 0;
	virtual void  setY(float y) = 0;
	virtual void  setZ(float z) = 0;
	virtual void  setW(float w) = 0;
	virtual void  setH(float h) = 0;
	virtual void  setRad(float r) = 0;
	virtual void  setDeg(float d) = 0;
	virtual void  setId(int id) = 0;
	virtual void  setTime(long t) = 0;
	virtual void  setValid(bool v) = 0;

	virtual float getX() = 0;
	virtual float getY() = 0;
	virtual float getZ() = 0;
	virtual float getW() = 0;
	virtual float getH() = 0;
	virtual float getRad() = 0;
	virtual float getDeg() = 0;
	virtual int   getId() = 0;
	virtual long  getTime() = 0;
	virtual bool  getValid() = 0;

	virtual float hasX() = 0;
	virtual float hasY() = 0;
	virtual float hasZ() = 0;
	virtual float hasW() = 0;
	virtual float hasH() = 0;
	virtual float hasRad() = 0;
	virtual float hasDeg() = 0;
	virtual float hasTime() = 0;

    /**
     * The methode operate() must be implemented by every derivative of this class.
     */
    virtual void operate();
};

#endif // ITRACKEDCOMPONENT_H
