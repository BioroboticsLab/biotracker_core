#ifndef TRACKEDELEMENT_H
#define TRACKEDELEMENT_H

#include "QGraphicsItem"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include "QString"
#include "Model/TrackedComponents/pose/FishPose.h"

/**
* This class is an example of how a TrackedComponent could be defined.
* This class inherits from the IModelTrackedComponent class and is therefor part of the Composite Pattern.
* This class represents the Leaf class in the Composite Pattern.
* Objects of this class have a QObject as parent.
*/
class TrackedElement : public IModelTrackedPoint
{
	Q_OBJECT

public:
	TrackedElement(QObject *parent = 0, QString name = "n.a.", int id = 0);

	QString getName();

	void  setX(float val);
	void  setY(float val);
	void  setZ(float z) {};
	void  setW(float w);
	void  setH(float h);
	void  setRad(float r);
	void  setDeg(float d);
	void  setId(int val) { _id = val; };
	void  setTime(std::chrono::steady_clock::time_point t) { _time = t; };
	void  setValid(bool v) { _valid = v; };

	float getX();
	float getY();
	float getZ() { return 0; };
	float getW() { return _w; };
	float getH() { return _h; };
	float getRad() { return _rad; };
	float getDeg();
	int   getId() { return _id; };
	std::chrono::steady_clock::time_point  getTime() { return _time; };
	bool  getValid() { return _valid; };

	bool hasX() { return true; };
	bool hasY() { return true; };
	bool hasZ() { return false; };
	bool hasW() { return true; };
	bool hasH() { return true; };
	bool hasRad() { return true; };
	bool hasDeg() { return true; };
	bool hasTime() { return true; };

	void pressed();
	void notPressed();
	bool getPressedStatus();

	void setFishPose(FishPose p);
	FishPose getFishPose();

	// ITrackedPoint interface
public:
	void operate();

private:
	QString _name;
	FishPose _pose;
	float _x;
	float _y;
	float _w;
	float _h;
	float _deg;
	float _rad;
	int _id;
	std::chrono::steady_clock::time_point _time;
	bool _valid;
	bool _pressed;
};

#endif // TRACKEDELEMENT_H
