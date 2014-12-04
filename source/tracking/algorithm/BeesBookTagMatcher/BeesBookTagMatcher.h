#ifndef BeesBookTagMatcher_H
#define BeesBookTagMatcher_H

#include <opencv2/opencv.hpp>

#include <boost/optional.hpp>

#include "source/tracking/TrackingAlgorithm.h"
#include "source/tracking/algorithm/BeesBookTagMatcher/resources/Grid.h"

//Class used to generate ground truth data for BeesBook
class BeesBookTagMatcher : public TrackingAlgorithm
{
private:
	std::shared_ptr<Grid> _activeGrid;
	boost::optional<ulong>     _activeFrameNumber;

	bool _ready;     // Ready for a new tag --Ctrl + LCM--
	bool _setTag;    // a new set of points is being configured
	bool _setP0;     // Set P0 --Left Click and drag--
	bool _setP1;     // Set P1 --Left Click and drag--
	bool _setP2;     // Set P2 --Left Click and drag--
	bool _setOrient; // to modify exclusively the tag orientation.

	std::vector<cv::Point> _orient; // auxiliar variable for drawing the orientation while setting the Tag

	// function that draws the Tags set so far calling instances of Grid.
	void drawSetTags(cv::Mat &image);

	// function that draws the orientation vector while being set.
	void drawOrientation(cv::Mat& image, const std::vector<cv::Point>& _orient);

	// function that draws an active tag calling an instance of Grid
	void drawActiveTag(cv::Mat& image);

	// function that draws the tag while being rotated in space
	void drawSettingTheta(cv::Mat &img);

	// function called while setting the tag (it initializes orient vector)
	void setTag(cv::Point location);

	// function that allows P1 and P2 to be modified to calculate the tag's angle in space.
	void setTheta(cv::Point location);

	// function that checks if one of the set Points is selected, returns true when one of the points is selected
	bool selectPoint(cv::Point location);

	// function that checks if one of the already set Tags is selected.
	void selectTag(cv::Point location);

	// store currently active tag in serialzation object list
	void storeCurrentActiveTag();

	// set no tas as currently active
	void cancelTag();

	// function that calculates the distance between two points
	double dist(cv::Point p1, cv::Point p2);

public:
	BeesBookTagMatcher(Settings &settings, std::string &serializationPathName, QWidget *parent);
	~BeesBookTagMatcher();

	void track(ulong frameNumber, cv::Mat & frame);
	void paint(cv::Mat& image);
	void reset();

public slots:
	void mouseMoveEvent(QMouseEvent * e);
	void mousePressEvent(QMouseEvent * e);
	void mouseReleaseEvent(QMouseEvent * e);
	void mouseWheelEvent(QWheelEvent * e);
};

#endif
