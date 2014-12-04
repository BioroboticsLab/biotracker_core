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
	, _currentState(State::Ready)
	, _setOnlyOrient(false)
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
	if (_currentState == State::SetTag) {
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
				if (_currentState == State::SetP1)
					_setOnlyOrient = true;
			}
		}
		//check for CTRL modifier
		else if (Qt::ControlModifier == QApplication::keyboardModifiers())    //a new tag is generated
		{
			if (_currentState == State::Ready) {
				setTag(cv::Point(e->x(), e->y()));
				_currentState = State::SetTag;
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
	switch (_currentState) {
	case State::SetTag:
		_orient[1] = cv::Point(e->x(), e->y());
		break;
	case State::SetP0:
		_activeGrid->translation(cv::Point(e->x(), e->y()));
		break;
	case State::SetP1:
		if (_setOnlyOrient) _activeGrid->orientation(cv::Point(e->x(), e->y()));
		else setTheta(cv::Point(e->x(), e->y()));
		break;
	case State::SetP2:
		setTheta(cv::Point(e->x(), e->y()));
		break;
	default:
		return;
	}
	emit update();
}

//check if MOUSE BUTTON IS RELEASED
void BeesBookTagMatcher::mouseReleaseEvent(QMouseEvent * e) {
	// left button released
	if (e->button() == Qt::LeftButton) {
		switch (_currentState) {
		//center and orientation of the tag were set.
		case State::SetTag:
			_activeGrid        = std::make_shared<Grid>(_orient[0], getAlpha(), _trackedObjects.size());
			_activeFrameNumber = _currentFrameNumber;
			//length of the vector is taked into consideration
			setTheta(cv::Point(e->x(), e->y()));
			_currentState = State::Ready;
			break;
		//the tag was translated
		case State::SetP0:
			_currentState = State::Ready;
			_activeGrid->updateVectors();
			break;
		//orientation of the bee and the marker were modified
		case State::SetP1:
			_currentState = State::Ready;
			_setOnlyOrient = false;
			_activeGrid->updateVectors();
			break;
		//orientation of the marker was modified
		case State::SetP2:
			//TODO
			//_setP2 = false;
			_currentState = State::Ready;
			_activeGrid->updateVectors();
			break;
		default:
			break;
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
	cv::line(image, orient[0], orient[1], cv::Scalar(0, 0, 255), 1);        //the orientation vector is printed in red
}

//function that draws an active tag calling an instance of Grid
void BeesBookTagMatcher::drawActiveTag(cv::Mat &image)
{
	_activeGrid->drawFullTag(image,1);         //the grid is drawn as active
	_activeGrid->updatePoints();
	for (int i = 0; i < 3; i++)
	{
		cv::circle(image, _activeGrid->absPoints[i], 1, COLOR_RED, 1);                 //the point is drawn in red
	}
	//active point in blue

	switch (_currentState) {
	case State::SetP0:
		cv::circle(image, _activeGrid->absPoints[0], 1, COLOR_BLUE, 1);
		break;
	case State::SetP1:
		cv::circle(image, _activeGrid->absPoints[1], 1, COLOR_BLUE, 1);
		break;
	case State::SetP2:
		cv::circle(image, _activeGrid->absPoints[2], 1, COLOR_BLUE, 1);
		break;
	default:
		break;
	}
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
	switch (_currentState) {
	case State::SetP1:
		cv::circle(img, _activeGrid->absPoints[0] + _activeGrid->realCoord[1], 1, COLOR_BLUE, 1);
		break;
	case State::SetP2:
		cv::circle(img, _activeGrid->absPoints[0] + _activeGrid->realCoord[2], 1, COLOR_BLUE, 1);
		break;
	default:
		break;
	}
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

	_currentState = State::SetTag;

	_orient.clear();
	_orient.push_back(location);
	_orient.push_back(location);
	emit update();          //the orientation vector is drawn.
	return;
}

//function that allows P1 and P2 to be modified to calculate the tag's angle in space.
void BeesBookTagMatcher::setTheta(cv::Point location) {
	double prop;
	double angle;

	switch (_currentState) {
	case State::SetP1:
		//the length of the vector is equal to the distance to the pointer, limited to axisTag and 0.5*axisTag
		prop = dist(_activeGrid->absPoints[0], location) / (_activeGrid->scale * BeesBookTag::AXISTAG);
		if (prop > 1)
			prop = 1;
		else if (prop < 0.5)
			prop = 0.5;
		//P1 updates the orientation of the tag too.
		_activeGrid->orientation(location);
		_activeGrid->realCoord[1] = _activeGrid->polar2rect(prop * BeesBookTag::AXISTAG, _activeGrid->alpha);
		break;
	case State::SetP2:
		//the length of the vector is equal to the distance to the pointer, limited to axisTag and 0.5*axisTag
		prop = dist(_activeGrid->absPoints[0], location) / (_activeGrid->scale * BeesBookTag::AXISTAG);
		if (prop > 1)
			prop = 1;
		else if (prop < 0.5)
			prop = 0.5;
		//P2 doesn't update alpha
		angle = atan2(location.x - _activeGrid->centerGrid.x, location.y - _activeGrid->centerGrid.y) - M_PI / 2;
		_activeGrid->realCoord[2] = _activeGrid->polar2rect(prop * BeesBookTag::AXISTAG, angle);
		break;
	default:
		break;
	}
	//updates parameters
	_activeGrid->updateParam();
	return;
}

//function that checks if one of the set Points is selected, returns true when one of the points is selected
bool BeesBookTagMatcher::selectPoint(cv::Point location) {
	for (int i = 0; i < 3; i++) {
		if (dist(location, _activeGrid->absPoints[i]) < 2) //check if the pointer is on one of the points
		{
			switch (i)
			{
			case 0:
				_currentState = State::SetP0;
				return true;
			case 1:
				_currentState = State::SetP1;
				return true;
			case 2:
				_currentState = State::SetP2;
				return true;
			}
		}
	}
	return false;
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
	_currentState  = State::Ready;
	_setOnlyOrient = false;

	emit update();
}

//AUXILIAR FUNCTION

//function that calculates the distance between two points
double BeesBookTagMatcher::dist(cv::Point p1, cv::Point p2) const
{
	const cv::Point diff = p1 - p2;
	return sqrt(diff.x * diff.x + diff.y * diff.y);
}

double BeesBookTagMatcher::getAlpha() const
{
	return atan2(_orient[1].x - _orient[0].x, _orient[1].y - _orient[0].y) - M_PI / 2;
}
