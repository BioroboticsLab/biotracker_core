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
    QString getCoordinateUnit() override { return "px"; };

    void  setX(float val);
    void  setY(float val);
    void  setW(float w) {};
    void  setH(float h) {};
    void  setXpx(float val) {};
    void  setYpx(float val) {};
    void  setWpx(float w) {};
    void  setHpx(float h) {};
	void  setRad(float r) {};
	void  setDeg(float d) {};
	void  setId(int val) { _id = val; };
	void  setTime(std::chrono::steady_clock::time_point t) { _time = t; };
	void  setValid(bool v) { _valid = v; };
	void  setFixed(bool f) { _fixed = f; };

    float getX();
    float getY();
    float getXpx();
    float getYpx();
    float getW() { return 0; };
    float getH() { return 0; };
    float getWpx() { return 0; };
    float getHpx() { return 0; };
	float getRad() { return 0; };
	float getDeg() { return 0; };
	int   getId() { return _id; };
	std::chrono::steady_clock::time_point  getTime() { return _time; };
	bool  getValid() { return _valid; };
	bool  getFixed() { return _fixed; };

	bool hasX() { return true; };
	bool hasY() { return true; };
	bool hasW() { return false; };
	bool hasH() { return false; };
    bool hasXpx() { return false; };
    bool hasYpx() { return false; };
    bool hasWpx() { return false; };
    bool hasHpx() { return false; };
	bool hasRad() { return false; };
	bool hasDeg() { return false; };
	bool hasTime() { return true; };

	void pressed();
	void notPressed();
	bool getPressedStatus();

	void setPoint(cv::Point2f p);
    cv::Point2f getPoint();

	// ITrackedPoint interface
public:
	void operate();

private:
	QString _name;
	float _x;
	float _y;
	int _id;
	std::chrono::steady_clock::time_point _time;
	bool _valid;
	bool _pressed;
	bool _fixed;
};

#endif // TRACKEDELEMENT_H
