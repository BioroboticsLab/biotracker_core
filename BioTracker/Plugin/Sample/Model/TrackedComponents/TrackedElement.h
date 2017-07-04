#ifndef TRACKEDELEMENT_H
#define TRACKEDELEMENT_H

#include "QGraphicsItem"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include "QString"

/**
 * This class is an example of how a TrackedComponent could be defined.
 * This class inherits from the IModelTrackedComponent class and is therefor part of the Composite Pattern.
 * This class represents the Leaf class in the Composite Pattern.
 * Objects of this class have a QObject as parent.
 */
class TrackedElement : public IModelTrackedComponent
{
    Q_OBJECT

  public:
    TrackedElement(QObject *parent = 0, QString name = "n.a.", int id=0);

    QString getName();

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
	float hasTime() { return true; };

    void pressed();
    void notPressed();
    bool getPressedStatus();

    // ITrackedComponent interface
public:
    void operate();

private:
    QString _name;
	int _id;
	long _time;
	bool _valid;

    bool _pressed;
};

#endif // TRACKEDELEMENT_H
