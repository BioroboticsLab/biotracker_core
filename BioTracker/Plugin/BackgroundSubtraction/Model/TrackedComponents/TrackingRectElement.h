#pragma once

#include "QGraphicsItem"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include "QString"

class TrackingRectElement : public IModelTrackedPoint
{
	Q_OBJECT

public:
	TrackingRectElement(QObject *parent = 0, QString name = "Rect corner", int id = 0) : _id(id) {
		//_rect = std::make_shared<QRectF>(0,0, 20, 20);
	};

	QString getName() { return "Rect corner"; };

	void  setX(float val) { _x = val; };
	void  setY(float val) { _y = val; };
	void  setZ(float z) {};
	void  setW(float w) {};
	void  setH(float h) {};
	void  setRad(float r) {};
	void  setDeg(float d) {};
	void  setId(int val) { _id = val; };
	void  setTime(std::chrono::steady_clock::time_point t) {};
	void  setValid(bool v) {};
	void  setFixed(bool f) { _fixed = f; };

	float getX() { return _x; };
	float getY() { return _y; };
	float getZ() { return 0; };
	float getW() { return 0; };
	float getH() { return 0; };
	float getRad() { return 0; };
	float getDeg() { return 0; };
	int   getId() { return _id; };
	std::chrono::steady_clock::time_point  getTime() { return std::chrono::steady_clock::time_point(); };
	bool  getValid() { return false; };
	bool  getFixed() { return _fixed; };

	bool hasX() { return true; };
	bool hasY() { return true; };
	bool hasZ() { return false; };
	bool hasW() { return false; };
	bool hasH() { return false; };
	bool hasRad() { return false; };
	bool hasDeg() { return false; };
	bool hasTime() { return false; };

	void updateXY(int x, int y) {
		_x = _x + x;
		_y = _y + y; 
	};

	void pressed();
	void notPressed();
	bool getPressedStatus();

	// ITrackedComponent interface
public:
	void operate() {};

private:
	int _id;
	float _x;
	float _y;
	bool _pressed;
	bool _fixed;
};
