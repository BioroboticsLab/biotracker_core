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
	virtual void add(IModelTrackedComponent *comp, int pos) = 0;

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

    virtual IModelTrackedComponent* getValidChild(int index) = 0;

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
	void  setTime(std::chrono::steady_clock::time_point t) { _time = t; };
	void  setValid(bool v) { _valid = v; };

	float getX() { return 0; };
	float getY() { return 0; };
	float getZ() { return 0; };
	float getW() { return 0; };
	float getH() { return 0; };
	float getRad() { return 0; };
	float getDeg() { return 0; };
	int   getId() { return _id; };
	std::chrono::steady_clock::time_point  getTime() { return _time; };
	bool  getValid() { return _valid; };

	bool hasX() { return false; };
	bool hasY() { return false; };
	bool hasZ() { return false; };
	bool hasW() { return false; };
	bool hasH() { return false; };
	bool hasRad() { return false; };
	bool hasDeg() { return false; };
	bool hasTime() { return false; };

	// ITrackedComponent interface
public:
	void operate();
	int _id;
	std::chrono::steady_clock::time_point _time;
	bool _valid;

protected:
	static int nextID;
};

#endif // ITRACKEDOTRAJECTORY_H
