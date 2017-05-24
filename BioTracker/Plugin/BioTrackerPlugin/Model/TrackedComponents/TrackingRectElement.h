#pragma once

#include "QGraphicsItem"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include "QString"

class TrackingRectElement : public IModelTrackedComponent
{
	Q_OBJECT

public:
	TrackingRectElement(QObject *parent = 0, QString name = "Rect corner", int id = 0) : _id(id), _pressed(false) {
		//_rect = std::make_shared<QRectF>(0,0, 20, 20);
	};

	QString getName() { return "Rect corner"; };

	void setId(int val) { _id = val; };
	void setX(int val) { _x = val; };
	void setY(int val) { _y = val; };
	void updateXY(int x, int y) {
		_x = _x + x;
		_y = _y + y; 
	};

	void pressed();
	void notPressed();

	int getId() { return _id; };
	int getX() { return _x; };
	int getY() { return _y; };
	/*std::shared_ptr<QRectF> getRect() {
		_rect->setX(_x - 10);
		_rect->setY(_y - 10);
		return _rect;
	};*/
	bool getPressedStatus() { return _pressed; };

	// ITrackedComponent interface
public:
	void operate() {};

private:
	int _id;
	int _x;
	int _y;

	//std::shared_ptr<QRectF> _rect;

	bool _pressed;
};
