#include "BeesBookTagMatcher.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include <QApplication>

#include "Common.h"
#include "source/tracking/algorithm/algorithms.h"

namespace {
auto _ = Algorithms::Registry::getInstance().register_tracker_type<
    BeesBookTagMatcher>("BeesBook Tag Matcher");

static const cv::Scalar COLOR_BLUE   = cv::Scalar(255, 0, 0);
static const cv::Scalar COLOR_RED    = cv::Scalar(0, 0, 255);
static const cv::Scalar COLOR_GREEN  = cv::Scalar(0, 255, 0);
static const cv::Scalar COLOR_YELLOW = cv::Scalar(0, 255, 255);
}

BeesBookTagMatcher::BeesBookTagMatcher(Settings & settings, std::string &serializationPathName, QWidget *parent)
	: TrackingAlgorithm(settings, serializationPathName, parent)
	, _ready(true)          //Ready for a new tag --Ctrl + LCM --
	, _setTag(false)        //State while the tag is being set, a red vector is drawn
	, _setP0(false)         //Set P0 --Left Click--
	, _setP1(false)         //Set P1 --Left Click--
	, _setP2(false)         //Set P2 --Left Click--
	, _setOrient(false)     //to modify exclusively the tag orientation.
{}

BeesBookTagMatcher::~BeesBookTagMatcher()
{
	storeCurrentActiveTag();
}

void BeesBookTagMatcher::track(ulong frameNumber, cv::Mat&)
{
	storeCurrentActiveTag();

	// copy all tags from last frame
	if (frameNumber > 0) {
		for (TrackedObject& trackedObject : _trackedObjects) {
			if (!(trackedObject.count(frameNumber)) && trackedObject.count(frameNumber - 1)) {
				std::shared_ptr<Grid> grid = trackedObject.get<Grid>(frameNumber - 1);
				trackedObject.add(frameNumber, std::make_shared<Grid>(*grid));
			}
		}
	}
}

void BeesBookTagMatcher::paint(cv::Mat& image)
{
	if (!_trackedObjects.empty()) {
		drawSetTags(image);
	}
	if (_setTag) {
		drawOrientation(image, _orient);
	} else if (_activeGrid)  {
		drawActiveTag(image);
	}
}
void BeesBookTagMatcher::reset() {
}

//check if MOUSE BUTTON IS CLICKED
void BeesBookTagMatcher::mousePressEvent(QMouseEvent * e) {
	//check if LEFT button is clicked
	if (e->button() == Qt::LeftButton) {
		//check for SHIFT modifier
		if (Qt::ShiftModifier == QApplication::keyboardModifiers())
		{
			if (_activeGrid)    // The Tag is active and can now be modified
			{
				//if clicked on one of the set points, the point is activated
				if (selectPoint(cv::Point(e->x(), e->y())))
					emit update();
				if (_setP1)
					_setOrient = true;
			}
		}
		//check for CTRL modifier
		else if (Qt::ControlModifier == QApplication::keyboardModifiers())    //a new tag is generated
		{
			if (_ready) {
				setTag(cv::Point(e->x(), e->y()));
				_setP1 = true;
			}
		}
		//without modifier
		else {
			if (_activeGrid) {                         // The Tag is active and can now be modified
				//if clicked in one of the bit cells, its value is changed
				const double distance = dist(_activeGrid->centerGrid, cv::Point(e->x(), e->y()));
				if ((distance > 2) && (distance < _activeGrid->axesGrid.height))
					_activeGrid->updateID(cv::Point(e->x(), e->y()));
				else
				//if clicked on one of the set points, the point is activated
				if (selectPoint(cv::Point(e->x(), e->y())))
					emit update();
				//otherwise checks if one of the other tags is selected
				else
					selectTag(cv::Point(e->x(), e->y()));
			} else {
				selectTag(cv::Point(e->x(), e->y()));
			}
		}
	}
	//check if RIGHT button is clicked
	if (e->button() == Qt::RightButton) {
		//check for SHIFT modifier
		if (Qt::ShiftModifier == QApplication::keyboardModifiers()) {
		}
		//check for CTRL modifier
		if (Qt::ControlModifier == QApplication::keyboardModifiers()) {
			cancelTag();                         //The Tag being currently configured is cancelled
		}
	}
}

//check if pointer MOVES
void BeesBookTagMatcher::mouseMoveEvent(QMouseEvent * e) {
	if (_setTag)         //Orientation of the tag is being set
	{
		_orient[1] = cv::Point(e->x(), e->y());
		emit update();
	} else if (_setP0)         //The tag is being translated
	{
		_activeGrid->translation(cv::Point(e->x(), e->y()));
		emit update();
	} else if (_setP1)         //The orientation of the tag is being modified
	{
		if (_setOrient)
			_activeGrid->orientation(cv::Point(e->x(), e->y()));
		else
			setTheta(cv::Point(e->x(), e->y()));
		emit update();
	} else if (_setP2)         //The space angle (theta) is being modified
	{
		setTheta(cv::Point(e->x(), e->y()));
		emit update();
	}
}

//check if MOUSE BUTTON IS RELEASED
void BeesBookTagMatcher::mouseReleaseEvent(QMouseEvent * e) {
	// left button released
	if (e->button() == Qt::LeftButton) {
		if (_setTag)                 //center and orientation of the tag were set.
		{
			_setTag = false;
			_ready = true;
			const double alpha = atan2(_orient[1].x - _orient[0].x, _orient[1].y - _orient[0].y) - M_PI / 2;
			_activeGrid        = std::make_shared<Grid>(_orient[0], alpha, _trackedObjects.size());
			_activeFrameNumber = _currentFrameNumber;

			//length of the vector is taked into consideration
			setTheta(cv::Point(e->x(), e->y()));
			_setP1 = false;
		} else if (_setP0)                 //the tag was translated
		{
			_setP0 = false;
			_activeGrid->updateVectors();
		}
		else if (_setP1)                 //orientation of the bee and the marker were modified
		{
			_setP1     = false;
			_setOrient = false;
			_activeGrid->updateVectors();
		}
		else if (_setP2)                 //orientation of the marker was modified
		{
			_setP2 = false;
			_activeGrid->updateVectors();
		}
		emit update();
	}
	// right button released
	if (e->button() == Qt::RightButton) {
	}
}

//check if WHEEL IS ACTIVE
void BeesBookTagMatcher::mouseWheelEvent        (QWheelEvent * e)
{
	if (_activeGrid)         // The Grid is active for draging
	{
		_activeGrid->scale = _activeGrid->scale + e->delta() / 96 * 0.05;                               //scale variable is updated by 0.05
		_activeGrid->updateAxes();
		emit update();
	}
}

//BeesBookTagMatcher private member functions

//DRAWING FUNCTIONS

//function that draws the set Tags so far.
void BeesBookTagMatcher::drawSetTags(cv::Mat& image)
{
	for (const TrackedObject& trackedObject : _trackedObjects) {
		if (trackedObject.count(_currentFrameNumber)) {
			std::shared_ptr<Grid> grid = trackedObject.get<Grid>(_currentFrameNumber);
			grid->drawFullTag(image, 2);
		}
	}
}

//function that draws the orientation vector while being set.
void BeesBookTagMatcher::drawOrientation(cv::Mat &image, const std::vector<cv::Point>& orient)
{
	//std::cout << "DRAW ORIENTAION" << std::endl;
	cv::line(image, orient[0], orient[1], cv::Scalar(0, 0, 255), 1);        //the orientation vector is printed in red
}

//function that draws an active tag calling an instance of Grid
void BeesBookTagMatcher::drawActiveTag(cv::Mat &image)
{
	//std::cout<<"DRAW ACTIVE GRID"<<std::endl;
	_activeGrid->drawFullTag(image,1);         //the grid is drawn as active
	_activeGrid->updatePoints();
	for (int i = 0; i < 3; i++)
	{
		cv::circle(image, _activeGrid->absPoints[i], 1, COLOR_RED, 1);                 //the point is drawn in red
	}
	//active point in blue

	if (_setP0)
		cv::circle(image, _activeGrid->absPoints[0], 1, COLOR_BLUE, 1);
	else if (_setP1)
		cv::circle(image, _activeGrid->absPoints[1], 1, COLOR_BLUE, 1);
	else if (_setP2)
		cv::circle(image, _activeGrid->absPoints[2], 1, COLOR_BLUE, 1);
}

//function that draws the tag while being rotated in space
void BeesBookTagMatcher::drawSettingTheta(cv::Mat &img)
{
	cv::ellipse(img, _activeGrid->absPoints[0], _activeGrid->axesTag, _activeGrid->angleTag, 0, 360, COLOR_YELLOW, 1);
	cv::line(img, _activeGrid->absPoints[0], _activeGrid->absPoints[0] + _activeGrid->realCoord[1], COLOR_RED, 1);          //the orientation vector is printed in red
	cv::line(img, _activeGrid->absPoints[0], _activeGrid->absPoints[0] + _activeGrid->realCoord[2], COLOR_GREEN, 1);        //the redius vector is printed in green

	for (int i = 0; i < 3; i++)
	{
		cv::circle(img, _activeGrid->absPoints[0] + _activeGrid->realCoord[i], 1, COLOR_RED, 1);               //the point is drawn in red
	}
	if (_setP1)
		cv::circle(img, _activeGrid->absPoints[0] + _activeGrid->realCoord[1], 1, COLOR_BLUE, 1);
	else if (_setP2)
		cv::circle(img, _activeGrid->absPoints[0] + _activeGrid->realCoord[2], 1, COLOR_BLUE, 1);
}

//TAG CONFIGURATION FUNCTIONS

//function called while setting the tag (it initializes orient vector)
void BeesBookTagMatcher::setTag(cv::Point location) {
	//If there is an active Tag, this is pushed into the _Grids vector and a new Tag is generated
	if (_activeGrid) {
		TrackedObject object(_trackedObjects.size());
		object.add(_currentFrameNumber, _activeGrid);
		_trackedObjects.push_back(object);
	}

	_ready  = false;
	_setTag = true;

	_orient.clear();
	_orient.push_back(location);
	_orient.push_back(location);
	emit update();          //the orientation vector is drawn.
	return;
}

//function that allows P1 and P2 to be modified to calculate the tag's angle in space.
void BeesBookTagMatcher::setTheta(cv::Point location) {
	//std::cout << " --------------------------------------------------------------- " << std::endl;
	double prop;
	double angle;
	//when P1 is being modified
	if (_setP1) {
		prop = dist(_activeGrid->absPoints[0], location) / (_activeGrid->scale * BeesBookTag::AXISTAG);               //the length of the vector is equal to the distance to the pointer, limited to axisTag and 0.5*axisTag
		if (prop > 1)
			prop = 1;
		else if (prop < 0.5)
			prop = 0.5;
		//P1 updates the orientation of the tag too.
		_activeGrid->orientation(location);
		_activeGrid->realCoord[1] = _activeGrid->polar2rect(prop * BeesBookTag::AXISTAG, _activeGrid->alpha);
	}
	//when P2 is being modified
	else if (_setP2) {
		prop = dist(_activeGrid->absPoints[0], location) / (_activeGrid->scale * BeesBookTag::AXISTAG);               //the length of the vector is equal to the distance to the pointer, limited to axisTag and 0.5*axisTag
		if (prop > 1)
			prop = 1;
		else if (prop < 0.5)
			prop = 0.5;
		//P2 doesn't update alpha
		angle = atan2(location.x - _activeGrid->centerGrid.x, location.y - _activeGrid->centerGrid.y) - M_PI / 2;
		_activeGrid->realCoord[2] = _activeGrid->polar2rect(prop * BeesBookTag::AXISTAG, angle);
		//std::cout << "realCoord 2: x = " << _activeGrid->realCoord[2].x << ", y = " << _activeGrid->realCoord[2].y << "(New value)" << std::endl;
	}
	//updates parameters
	_activeGrid->updateParam();
	return;
}

//function that checks if one of the set Points is selected, returns true when one of the points is selected
bool BeesBookTagMatcher::selectPoint(cv::Point location) {
	bool answer = false;
	for (int i = 0; i < 3; i++) {
		if (dist(location, _activeGrid->absPoints[i]) < 2)               //check if the pointer is on one of the points
		{
			switch (i)
			{
			case 0:
				_setP0 = true;
				answer = true;
				break;
			case 1:
				_setP1 = true;
				answer = true;
				break;
			case 2:
				_setP2 = true;
				answer = true;
				break;
			default:
				answer = false;
				break;
			}
			break;
		}
	}
	return answer;
}

//function that checks if one of the already set Tags is selected.
void BeesBookTagMatcher::selectTag(cv::Point location)
{
	for (const TrackedObject& trackedObject : _trackedObjects) {
		std::shared_ptr<Grid> grid = trackedObject.maybeGet<Grid>(_currentFrameNumber);
		if (grid && dist(location, grid->centerGrid) < grid->axesTag.height) {
			storeCurrentActiveTag();
			_activeGrid        = grid;
			_activeFrameNumber = _currentFrameNumber;
			emit update();
		}
	}
}

void BeesBookTagMatcher::storeCurrentActiveTag()
{
	// store last active grid
	if (_activeGrid) {
		if (_activeGrid->_objectId == _trackedObjects.size()) {
			assert(_activeFrameNumber);
			TrackedObject newObject(_trackedObjects.size());
			newObject.add(_activeFrameNumber.get(), _activeGrid);
			_trackedObjects.push_back(std::move(newObject));
		} else {
			assert(_activeFrameNumber);
			assert(_activeGrid->_objectId < _trackedObjects.size());
			_trackedObjects[_activeGrid->_objectId].add(_activeFrameNumber.get(), _activeGrid);
		}
		// set tag as not active
		cancelTag();
	}
}

void BeesBookTagMatcher::cancelTag()
{
	_activeGrid.reset();
	_activeFrameNumber.reset();
	//TODO: simplify
	_ready     = true;
	_setTag    = false;
	_setP0     = false;
	_setP1     = false;
	_setP2     = false;
	_setOrient = false;

	emit update();
}

//AUXILIAR FUNCTION

//function that calculates the distance between two points
double BeesBookTagMatcher::dist(cv::Point p1, cv::Point p2)
{
	const cv::Point diff = p1 - p2;
	return sqrt(diff.x * diff.x + diff.y * diff.y);
}
