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

	virtual int validCount() = 0;


	/**
	* This shoudd simply return a last child object (highest index).
	*/
	virtual IModelTrackedComponent *getLastChild() = 0;

	void  setTime(std::chrono::system_clock::time_point t) { _time = t; };
	std::chrono::system_clock::time_point  getTime() { return _time; };

	// ITrackedComponent interface
public:
	void operate();
	std::chrono::system_clock::time_point _time;

protected:
	static int nextID;
};

#endif // ITRACKEDOTRAJECTORY_H
