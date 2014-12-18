#ifndef BeesBookTagMatcher_H
#define BeesBookTagMatcher_H

#include <chrono>
#include <cstdint>
#include <memory>

#include <opencv2/opencv.hpp>

#include <boost/optional.hpp>

#include "source/tracking/TrackingAlgorithm.h"
#include "source/tracking/algorithm/BeesBookTagMatcher/resources/Grid3D.h"

#include "ui_BeesBookTagMatcherToolWidget.h"

//Class used to generate ground truth data for BeesBook
class BeesBookTagMatcher : public TrackingAlgorithm {
	Q_OBJECT
private:

	static const size_t GRID_RADIUS_PIXELS;

	std::shared_ptr<Grid3D>	_activeGrid; // points to active grid (grid must be active to be altered)
	boost::optional<ulong>	_activeFrameNumber;
	boost::optional<size_t> _activeGridObjectId;

	enum class State : uint8_t {
		Ready = 0,  // Ready for a new tag --Ctrl + LCM--
		SetTag,     // A new tag is being drawn
		SetP0,      // Set P0 --Left Click and drag--
		SetP1,      // Set P1 --Left Click and drag--
		SetP2,      // Set P2 --Left Click and drag--
		SetBit,	    // Set single bits
	};

	State _currentState;  // current state of user interaction

	cv::Point2f _rotationAxis;  // unit vector that defines the tag's rotation in space
	cv::Point2f _tempPoint;     // temporary point for spatial rotation in user interaction

	typedef struct {
		cv::Point from;
		cv::Point to;

		double norm() const { return cv::norm(to - from); }
		double alpha() const { return atan2(to.y - from.y, to.x - from.x); }
	} Orientation;

	// auxiliar variable for drawing a line while setting the Tag
	Orientation _orient;

	std::chrono::system_clock::time_point _lastMouseEventTime;

	Ui::TagMatcherToolWidget _UiToolWidget;

	std::shared_ptr<QWidget> _toolWidget;
	std::shared_ptr<QWidget> _paramWidget;

	std::set<size_t>        _idCopyBuffer;
	boost::optional<size_t> _copyFromFrame;
    cv::Rect                _imgRect;       // stores image dimensions
    cv::Rect                _validRect;     // holds a rect that defines valid positions for tags (changes size when tags are resized)

	// function that draws the Tags set so far calling instances of Grid.
	void drawTags(cv::Mat &image) const;

	// function that draws the orientation vector while being set.
	void drawOrientation(cv::Mat& image, const Orientation& orient) const;

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

	// set no tas as currently active
	void cancelTag();

	// reset shared_ptr to current active grid and auxilary variables for active grid
	void resetActiveGrid();

	// remove active grid from current frame
	void removeCurrentActiveTag();

	// function that calculates the distance between two points
	double dist(const cv::Point& p1, const cv::Point& p2) const;

	// calculate number of tags on current frame
	void setNumTags();

	void forcePointIntoBorders(cv::Point & point, cv::Rect const & borders);

	void updateValidRect();

	cv::Scalar getGridColor(std::shared_ptr<Grid3D> const& grid) const;

	void mouseMoveEvent    (QMouseEvent * e) override;
	void mousePressEvent   (QMouseEvent * e) override;
	void mouseReleaseEvent (QMouseEvent * e) override;
	void keyPressEvent     (QKeyEvent * e) override;

protected:
	bool event(QEvent* event) override;

public:
	BeesBookTagMatcher(Settings &settings, QWidget *parent);
	~BeesBookTagMatcher();

	void track(ulong frameNumber, cv::Mat& frame) override;
	void paint(cv::Mat& image) override;
	void reset() override {}
	void postLoad() override;

	std::shared_ptr<QWidget> getToolsWidget() override { return _toolWidget; }
	std::shared_ptr<QWidget> getParamsWidget() override { return _paramWidget; }

	// return keys that are handled by the tracker
	std::set<Qt::Key> const& grabbedKeys() const override;
};

#endif
