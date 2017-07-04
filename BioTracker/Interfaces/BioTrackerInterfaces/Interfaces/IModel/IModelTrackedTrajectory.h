#ifndef ITRACKEDOTRAJECTORY_H
#define ITRACKEDOTRAJECTORY_H

#include "Interfaces/IModel/IModelTrackedComponent.h"

/**
 * This interface class derives from IModelTrackedComponent.
 * This class is part of the Composite Pattern and represents the the abstract Composite class.
 * Its purpose is to define the responsibility
 * for adding, deleting and returning leaf objects.
 *
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
	virtual bool remove(IModelTrackedComponent *comp) = 0;

	/**
	* This methode must be implemented by all derivatives. Its functionality shall be a mechanism for removing all Leaf components from this structure.
	*/
	virtual void clear() = 0;

	/**
	* This methode must be implemented by all derivatives. Its functionality shall be a mechanism for determining the count/number of children.
	*/
	virtual int size() = 0;

    /**
     * This should simply return a child object by an index.
     */
    virtual IModelTrackedComponent* getChild(int index)  = 0;

	/**
	* This shoudd simply return a last child object (highest index).
	*/
	virtual IModelTrackedComponent *getLastChild() = 0;

	void  setX(float val) {};
	void  setY(float val) {};
	void  setZ(float z) {};
	void  setW(float w) {};
	void  setH(float h) {};
	void  setRad(float r) {};
	void  setDeg(float d) {};
	void  setId(int val) { _id = val; };
	void  setTime(long t) { _time = t; };
	void  setValid(bool v) { _valid = v; };

	float getX() { return 0; };
	float getY() { return 0; };
	float getZ() { return 0; };
	float getW() { return 0; };
	float getH() { return 0; };
	float getRad() { return 0; };
	float getDeg() { return 0; };
	int   getId() { return _id; };
	long  getTime() { return _time; };
	bool  getValid() { return _valid; };

	float hasX() { return false; };
	float hasY() { return false; };
	float hasZ() { return false; };
	float hasW() { return false; };
	float hasH() { return false; };
	float hasRad() { return false; };
	float hasDeg() { return false; };
	float hasTime() { return false; };

    // ITrackedComponent interface
public:
    void operate();
	int _id;
	long _time;
	bool _valid;
};

#endif // ITRACKEDOTRAJECTORY_H
