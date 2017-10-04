#pragma once
#include "Interfaces/IModel/IModel.h"
#include <cv.h>
#include <QPoint>

class AreaInfoElement : public IModel
{
	Q_OBJECT

public:
	AreaInfoElement(int type = 0);
	~AreaInfoElement();

	bool insideElement(cv::Point p);

	void setVertices(std::vector<cv::Point> p) {
		_v = p;
		Q_EMIT updatedVertices();
	};

	std::vector<cv::Point> _v;

	std::vector<cv::Point> getVertices() { return _v; };

	//TODO: change everything to QPoints?
	std::vector<QPoint> getQVertices() { 
		std::vector<QPoint> v;
		for (int i = 0; i < _v.size(); i++) {
			v.push_back(QPoint(_v[i].x, _v[i].y));
		}
		return v; 
	};
	int _type;

	void setVerticeAtLocation(const QPoint &pos, int vertice);
	int getVerticeAtLocation(const QPoint &pos);

	virtual QPoint *getHandleForPosition(const QPoint &pos)
	{
		if (isHandleAtPosition(origin, pos)) return &origin;
		return nullptr;
	}
protected:
	bool isHandleAtPosition(const cv::Point2f &handle, const QPoint &pos);
	bool isHandleAtPosition(const QPoint &handle, const QPoint &pos);

signals:
	void updatedVertices();

private:
	// Position in pixels.
	QPoint origin{ 0, 0 };
};

