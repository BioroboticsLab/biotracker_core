#include "SimpleTracker.h"

#include <opencv.hpp>

class BgSub : public cv::BackgroundSubtractorMOG2 {
public:
	BgSub() {
		bShadowDetection = true;
		nShadowDetection = 0;
		fTau = 0.90;
		nmixtures = 60;
	}
};

SimpleTracker::SimpleTracker(Settings& settings) : TrackingAlgorithm(settings), _bg_subtractor(BgSub())
{
}


SimpleTracker::~SimpleTracker(void)
{
}

cv::Mat SimpleTracker::track(std::vector<TrackedObject> objectList, cv::Mat frame) {
	std::vector<std::vector<cv::Point> > contours;
	cv::Mat foreground;
	cv::Mat background;
	_bg_subtractor.operator ()(frame, foreground);
	_bg_subtractor.getBackgroundImage(background);

	cv::erode(foreground, foreground, cv::Mat());

	cv::dilate(foreground, foreground, cv::Mat());

	cv::findContours(foreground, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	std::vector<std::vector<cv::Point> > contours_poly(contours.size());
	std::vector<cv::Point2f> center(contours.size());
	float radius_dummy;
	for(int i = 0; i < contours.size(); i++) {
		cv::minEnclosingCircle(contours[i], center[i], radius_dummy);
	}

	cv::Mat canvas = frame;

	cv::RNG rng(12345);
	for(int i = 0; i < contours.size(); i++) {
		cv::Scalar color1(0, 0, 255);
		cv::Scalar color_rand(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
		cv::drawContours(canvas, contours, i, color1, 2, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
		cv::circle(canvas, center[i], 2, color_rand, -1, 8, 0 );
	}

	return canvas;
}

void SimpleTracker::reset() {
	_bg_subtractor = BgSub();
}
