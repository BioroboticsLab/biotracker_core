#include "AreaInfoElement.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QDebug>

AreaInfoElement::AreaInfoElement(int type) : 
    _areaType(BiotrackerTypes::AreaType::NONE),
    _type(type),
    _showNumbers(false)
{
}


AreaInfoElement::~AreaInfoElement()
{
}


bool AreaInfoElement::insideElement(cv::Point p) {

	if (_type == 0) {
		auto start = _v.begin();
		auto end = _v.begin() + 4;
		std::vector<cv::Point> rectV(start, end);
		return cv::pointPolygonTest(rectV, p, true) > 0;
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
	else if (_type == 2){
		return cv::pointPolygonTest(_v, p, true) > 0;
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
	else if (_type == 2) {
		int numberOfVertices = _v.size();
		for (int i = 0; i < numberOfVertices; i++) {
			if (isHandleAtPosition(_v[i], pos))
				return i;
		}
	}

	return -1;
}

//Moving vertices
void AreaInfoElement::setVerticeAtLocation(const QPoint &pos, int vertex) {
	int numberOfVertices = _v.size();
	//Move existing vertices
	if (_type == 0 && vertex >= 0 && vertex < 4) {
		_v[vertex] = cv::Point2f(pos.x(), pos.y());
	}
	else if (_type == 1 && vertex >= 0 && vertex < 2) {
		_v[vertex] = cv::Point2f(pos.x(), pos.y());
	}
	else if (_type == 2 && vertex >= 0 && vertex < numberOfVertices) {
		_v[vertex] = cv::Point2f(pos.x(), pos.y());
	}
	// //Add new vertex to polygon
	// else if (_type == 2 && vertex == numberOfVertices) {
	// 	_v.push_back(cv::Point2f(pos.x(), pos.y()));
	// }

	Q_EMIT updatedVertices();

}

void AreaInfoElement::changeTrArNumberOfVertices(int number) {
	int numberOfVertices = _v.size();
	
	// add more vertices
	while (numberOfVertices < number) {
		cv::Point2f first = _v[0];
		cv::Point2f last = _v[numberOfVertices-1];
		_v.push_back((first+last) * .5);
		numberOfVertices = _v.size();
	}

	// remove last vertices (down to 4)
	while (numberOfVertices > number && numberOfVertices > 4) {
		_v.pop_back();
		numberOfVertices = _v.size();
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
