#ifndef BeesBookTagMatcher_H
#define BeesBookTagMatcher_H

#include <chrono>
#include <cstdint>

#include <opencv2/opencv.hpp>

#include <boost/optional.hpp>

#include "source/tracking/TrackingAlgorithm.h"
#include "source/tracking/algorithm/BeesBookTagMatcher/resources/Grid.h"

//Class used to generate ground truth data for BeesBook
class BeesBookTagMatcher : public TrackingAlgorithm
{
private:
	std::shared_ptr<Grid>  _activeGrid;
	boost::optional<ulong> _activeFrameNumber;

	enum class State : uint8_t {
		Ready = 0, // Ready for a new tag --Ctrl + LCM--
		SetTag,    // a new set of points is being configured
		SetP0,     // Set P0 --Left Click and drag--
		SetP1,     // Set P1 --Left Click and drag--
		SetP2,     // Set P2 --Left Click and drag--
	};

	State _currentState;
	bool _setOnlyOrient; // to modify exclusively the tag orientation.

	std::vector<cv::Point> _orient; // auxiliar variable for drawing the orientation while setting the Tag

	std::chrono::system_clock::time_point _lastMouseEventTime;

	// function that draws the Tags set so far calling instances of Grid.
	void drawSetTags(cv::Mat &image) const;

	// function that draws the orientation vector while being set.
	void drawOrientation(cv::Mat& image, const std::vector<cv::Point>& _orient) const;

	// function that draws an active tag calling an instance of Grid
	void drawActiveTag(cv::Mat& image) const;

	// function that draws the tag while being rotated in space
	void drawSettingTheta(cv::Mat &img) const;

	// function called while setting the tag (it initializes orient vector)
	void setTag(const cv::Point& location);

	// function that allows P1 and P2 to be modified to calculate the tag's angle in space.
	void setTheta(const cv::Point& location);

	// function that checks if one of the set Points is selected, returns true when one of the points is selected
	bool selectPoint(const cv::Point& location);

	// function that checks if one of the already set Tags is selected.
	void selectTag(const cv::Point& location);

	// store currently active tag in serialzation object list
	void storeCurrentActiveTag();

	// set no tas as currently active
	void cancelTag();

	// function that calculates the distance between two points
	double dist(const cv::Point& p1, const cv::Point& p2) const;

	double getAlpha() const;

public:
	BeesBookTagMatcher(Settings &settings, std::string &serializationPathName, QWidget *parent);
	~BeesBookTagMatcher();

	void track(ulong frameNumber, cv::Mat& frame) override;
	void paint(cv::Mat& image) override;
	void reset() override;

public slots:
	void mouseMoveEvent(QMouseEvent * e) override;
	void mousePressEvent(QMouseEvent * e) override;
	void mouseReleaseEvent(QMouseEvent * e) override;
	void mouseWheelEvent(QWheelEvent * e) override;
};

#endif
