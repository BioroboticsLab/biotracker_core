#include "AreaInfoElement.h"

#include <cv.h>
#include "opencv2/imgproc/imgproc.hpp"

AreaInfoElement::AreaInfoElement(int type)
{
	_type = type;
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
