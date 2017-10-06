#include "AreaInfoElement.h"

#include <cv.h>
#include "opencv2/imgproc/imgproc.hpp"

AreaInfoElement::AreaInfoElement(int type)
{
	_type = type;
    _showNumbers = false;
}


AreaInfoElement::~AreaInfoElement()
{
}


bool AreaInfoElement::insideElement(cv::Point p) {

	if (_type == 0) {

		return cv::pointPolygonTest(_v, p, true) > 0;
	}
	else if (_type == 1) {
		float rx = std::abs(_v[1].x - _v[0].x) / 2;
		float ry = std::abs(_v[1].y - _v[0].y) / 2;
		float h = _v[0].x + rx;
		float k = _v[0].y + ry;
		float x = p.x;
		float y = p.y;

		float xh = std::pow(x - h, 2);
		float yk = std::pow(y - k, 2);
		float val = (xh / (rx*rx)) + (yk / (ry*ry));
		bool inShape = val <= 1;

		return inShape;
	}
	return false;
}

int AreaInfoElement::getVerticeAtLocation(const QPoint &pos) {
	if (_type == 0) {
		for (int i = 0; i < 4; i++) {
			if (isHandleAtPosition(_v[i], pos))
				return i;
		}
	}
	else if (_type == 1) {
		for (int i = 0; i < 2; i++) {
			if (isHandleAtPosition(_v[i], pos))
				return i;
		}
	}

	return -1;
}

void AreaInfoElement::setVerticeAtLocation(const QPoint &pos, int vertice) {
	if (_type == 0 && vertice >= 0 && vertice <4) {
		_v[vertice] = cv::Point2f(pos.x(), pos.y());
	}
	else if (_type > 0 && vertice >= 0 && vertice < 2) {
		_v[vertice] = cv::Point2f(pos.x(), pos.y());
	}

	Q_EMIT updatedVertices();

}

bool AreaInfoElement::isHandleAtPosition(const cv::Point2f &handle, const QPoint &pos) {
	return isHandleAtPosition(QPoint(handle.x, handle.y), pos);
}
bool AreaInfoElement::isHandleAtPosition(const QPoint &handle, const QPoint &pos)
{
	QPoint diff = handle - pos;
	const float euclidian = std::sqrt((diff.x() * diff.x()) + (diff.y() * diff.y()));
	return euclidian <= 20.0;
}
