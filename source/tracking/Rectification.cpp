#include "Rectification.h"
#include <algorithm> // std::max_element, std::min_element, std::min, std::max

Rectification::Rectification(void)
{}

Rectification::Rectification(double areaHeight_cm, double areaWidth_cm,
		std::vector<cv::Point> areaCoordinates, int camCaptureWidth_px,
		int camCaptureHeight_px) :
	_areaCoordinates(std::move(areaCoordinates)),
	_q(3, 1, CV_32F),
	_camCaptureWidth_px(camCaptureWidth_px),
	_camCaptureHeight_px(camCaptureHeight_px),
	_areaWidth_cm(areaWidth_cm),
	_areaHeight_cm(areaHeight_cm)
{
	std::vector<cv::Point2f> areaCoordinates_2f;
	cv::Mat(_areaCoordinates).copyTo(areaCoordinates_2f);

	_rectifiedAreaCoordinates.push_back(cv::Point2f(0.0,0.0));
	_rectifiedAreaCoordinates.push_back(cv::Point2f(0.0, _areaHeight_cm));
	_rectifiedAreaCoordinates.push_back(cv::Point2f(_areaWidth_cm, _areaHeight_cm));
	_rectifiedAreaCoordinates.push_back(cv::Point2f(_areaWidth_cm, 0.0));

	_H = cv::findHomography(areaCoordinates_2f, _rectifiedAreaCoordinates);
	_H_inv = _H.inv();
}

cv::Point2f Rectification::pxToCm(cv::Point point_px) const
{
	cv::Mat_<double> v = (cv::Mat_<double>(3,1) << point_px.x, point_px.y, 1);
	cv::gemm(_H, v, 1, 0, 0, _q);
	double* qx = _q.ptr<double>(0);
	double* qy = _q.ptr<double>(1);
	double* qz = _q.ptr<double>(2);

	return cv::Point2f((*qx / *qz), (*qy / *qz));
}

cv::Point2f Rectification::pxToCm(QPoint point_px) const
{
	return pxToCm(cv::Point(point_px.x(), point_px.y()));
}

cv::Point2f Rectification::cmToPx(cv::Point2f point_cm) const
{
	cv::Mat_<double> v = (cv::Mat_<double>(3, 1) << point_cm.x, point_cm.y, 1);
	cv::gemm(_H_inv, v, 1, 0, 0, _q);
	double *qx = _q.ptr<double>(0);
	double *qy = _q.ptr<double>(1);
	double *qz = _q.ptr<double>(2);

	return cv::Point2f((*qx / *qz), (*qy / *qz));
}

bool Rectification::inArea(cv::Point2f point_cm) const
{
	return (point_cm.x > 0 && point_cm.y > 0
		&& point_cm.x < _areaWidth_cm
		&& point_cm.y < _areaHeight_cm);
}


namespace {
  
  struct cvPoint_less_x {
    bool operator()(const cv::Point &lhs, const cv::Point &rhs) const {
      return lhs.x < rhs.x;
    }
  };
  
  struct cvPoint_less_y {
    bool operator()(const cv::Point &lhs, const cv::Point &rhs) const {
      return lhs.y < rhs.y;
    }
  };
  
}

int Rectification::minXPx() const
{
	if (_areaCoordinates.empty()) {
		return _camCaptureWidth_px;
	}
	const auto &it = std::min_element(_areaCoordinates.cbegin(), _areaCoordinates.cend(), cvPoint_less_x());
	return std::min(_camCaptureWidth_px, it->x);
}

int Rectification::maxXPx() const
{
	if (_areaCoordinates.empty()) {
	  return -1;
	}
	const auto &it = std::max_element(_areaCoordinates.cbegin(), _areaCoordinates.cend(), cvPoint_less_x());
	return it->x;
}

int Rectification::minYPx() const
{
	if (_areaCoordinates.empty()) {
		return _camCaptureHeight_px;
	}
	const auto &it = std::min_element(_areaCoordinates.cbegin(), _areaCoordinates.cend(), cvPoint_less_y());
	return std::min(_camCaptureHeight_px, it->y);
}

int Rectification::maxYPx() const
{
	if (_areaCoordinates.empty()) {
	  return -1;
	}
	const auto &it = std::max_element(_areaCoordinates.cbegin(), _areaCoordinates.cend(), cvPoint_less_y());
	return it->y;
}
