#pragma once

#include "QGraphicsItem"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include "QString"

class TrackingRectElement : public IModelTrackedComponent
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
	void  setTime(long t) {};
	void  setValid(bool v) {};

	float getX() { return _x; };
	float getY() { return _y; };
	float getZ() { return 0; };
	float getW() { return 0; };
	float getH() { return 0; };
	float getRad() { return 0; };
	float getDeg() { return 0; };
	int   getId() { return _id; };
	long  getTime() { return 0; };
	bool  getValid() { return false; };

	float hasX() { return true; };
	float hasY() { return true; };
	float hasZ() { return false; };
	float hasW() { return false; };
	float hasH() { return false; };
	float hasRad() { return false; };
	float hasDeg() { return false; };
	float hasTime() { return false; };

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
};
