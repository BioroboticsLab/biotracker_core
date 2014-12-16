#include "BeesBookTagMatcher.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include <QApplication>

#include "Common.h"
#include "source/tracking/algorithm/algorithms.h"
#include "utility/CvHelper.h"

namespace {
auto _ = Algorithms::Registry::getInstance().register_tracker_type<
    BeesBookTagMatcher>("BeesBook Tag Matcher");

static const cv::Scalar COLOR_BLUE   = cv::Scalar(255, 0, 0);
static const cv::Scalar COLOR_RED    = cv::Scalar(0, 0, 255);
static const cv::Scalar COLOR_GREEN  = cv::Scalar(0, 255, 0);
static const cv::Scalar COLOR_YELLOW = cv::Scalar(0, 255, 255);
}

BeesBookTagMatcher::BeesBookTagMatcher(Settings & settings, QWidget *parent)
	: TrackingAlgorithm(settings, parent)
	, _currentState(State::Ready)
	, _setOnlyOrient(false)
	, _lastMouseEventTime(std::chrono::system_clock::now())
	, _toolWidget(std::make_shared<QWidget>())
	, _paramWidget(std::make_shared<QWidget>())	
{
	_UiToolWidget.setupUi(_toolWidget.get());
	setNumTags();
}

BeesBookTagMatcher::~BeesBookTagMatcher()
{
}

void BeesBookTagMatcher::track(ulong /* frameNumber */, cv::Mat & /* frame */)
{
	_activeGrid.reset();
	setNumTags();
}

void BeesBookTagMatcher::paint(cv::Mat& image)
{
	if (!_trackedObjects.empty())
	{
		drawSetTags(image);
	}
	if (_currentState == State::SetTag)
	{
		drawOrientation(image, _orient);
	}
	else if (_activeGrid)
	{
		drawActiveTag(image);
	}
}
void BeesBookTagMatcher::reset() {
}

void BeesBookTagMatcher::postLoad()
{
	setNumTags();
}

//check if MOUSE BUTTON IS CLICKED
void BeesBookTagMatcher::mousePressEvent(QMouseEvent * e) 
{
	// position of mouse cursor 
	const cv::Point mousePosition(e->x(), e->y());
	const bool ctrlModifier  = QApplication::keyboardModifiers().testFlag(Qt::ControlModifier);
	const bool shiftModifier = QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier);
	
	// left mouse button down:
	// select tag among all visible tags
	// if there is a selected tag: select keypoint
	// if no keypoint selected: compute P2 = _center - p; set space rotation
	// LMB with CTRL: new tag
	// RMB without modifier: store click point temporarily, set rotation mode
	//check if LEFT button is clicked
	if (e->button() == Qt::LeftButton)
	{
		if (ctrlModifier)
		{
			if (_currentState == State::Ready)
			{
				_activeGrid.reset();
				setTag(cv::Point(e->x(), e->y()));
			}
		}
		else
		{
			if (_activeGrid)
			{
				int id = _activeGrid->getKeyPointIndex(mousePosition);
				const bool indeterminate = ctrlModifier && shiftModifier;

				if ((id >= 0) && (id < 12)) // ToDo: use constants
				{
					// change state, so consequent mouse moves do not move the tag when toggling bits
					_currentState = State::SetBit;

					_activeGrid->toggleIdBit(id, indeterminate);
				} else {
					switch (id)
					{
						case 12:
						{
							_currentState = State::SetP0;
							break;
						}
						case 13: // P1, ToDo: use constants
						{
							_currentState = State::SetP1;
							break;
						}
						default:
						{
							selectTag(mousePosition);
						}
					}
				}
			} else
			{
				selectTag(mousePosition);
			}
		}
	} else if (e->button() == Qt::RightButton)
	{
		if (ctrlModifier)
		{
			removeCurrentActiveTag();
		} else if (_activeGrid)
		{
			if (e->button() == Qt::RightButton)
			{
				// vector orthogonal to rotation axis
				_tempPoint = mousePosition - _activeGrid->getCenter();

				// set "rotation in space"-state
				_currentState = State::SetP2;
			}
		}
	}

	emit update();
}

//check if pointer MOVES
void BeesBookTagMatcher::mouseMoveEvent(QMouseEvent * e) 
{
	const cv::Point p(e->x(), e->y());
	const auto elapsed = std::chrono::system_clock::now() - _lastMouseEventTime;
	if (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() > 1) {
		switch (_currentState) 
		{
		case State::SetTag:
			_orient.to = p;
			break;
		case State::SetP0:
		{
			_activeGrid->setCenter(p);
			break;
		}
		case State::SetP1:
		{
			_activeGrid->zRotateTowardsPointInPlane(p);
			break;
		}		
		case State::SetP2:
		{
			// vector orthogonal to rotation axis 
			const cv::Point2f temp = p - _activeGrid->getCenter();

			// distance to center
			const float d0 = cv::norm(_tempPoint);
			const float d1 = cv::norm(temp);

			// the rotation axis in image reference frame (unit vector)
			const float x = -temp.y / d1;
			const float y =  temp.x / d1;

			// z - angle of grid
			const double a = _activeGrid->getZRotation();

			// the rotation axis in grid reference frame (ToDo: rotate in space?)
			_rotationAxis.x = cos(a) * x + sin(a) * y;
			_rotationAxis.y = -sin(a) * x + cos(a) * y;		
						
			// weight of rotation
			const float w = 0.05*(d0 - d1);
			
			_activeGrid->xyRotateIntoPlane(w * _rotationAxis.y, w * _rotationAxis.x);
						
			break;
		}
		default:
			return;
		}
		emit update();
		_lastMouseEventTime = std::chrono::system_clock::now();
	}
}

//check if MOUSE BUTTON IS RELEASED
void BeesBookTagMatcher::mouseReleaseEvent(QMouseEvent * e) 
{
	// left button released
	if (e->button() == Qt::LeftButton) {
		switch (_currentState) {
		//center and orientation of the tag were set.
		case State::SetTag:
		{
			// update active frame number and active grid
			_activeFrameNumber = _currentFrameNumber;

			const size_t newID = _trackedObjects.empty() ? 0 : _trackedObjects.back().getId() + 1;

			// insert new trackedObject object into _trackedObjects ( check if empty "first")
			_trackedObjects.emplace_back(newID);

			// associate new (active) grid to frame number
			// ToDo: fix angle
			_activeGrid = std::make_shared<Grid3D>(_orient.from, getRadiusFromFocalLength(), _orient.alpha(), 0., 0.);

			_trackedObjects.back().add(_currentFrameNumber, _activeGrid);

			//length of the vector is taken into consideration
			setTheta(cv::Point(e->x(), e->y()));
			_currentState = State::Ready;

			setNumTags();
			break;
		}
		case State::SetBit:
			_currentState = State::SetP0;
			break;
		default:
			break;
		}
		emit update();
	}
}

void BeesBookTagMatcher::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Plus || e->key() == Qt::Key_Minus) 
	{
		if (_activeGrid) 
		{
			const float direction = e->key() == Qt::Key_Plus ? 1.f : -1.f;
			//scale variable is updated by 0.05
			/*_activeGrid->scale = _activeGrid->scale + direction * 0.05;
			_activeGrid->updateAxes();*/
			emit update();
		}
	} 
	else 
		if (e->key() == Qt::Key_C && e->modifiers().testFlag(Qt::ControlModifier)) 
		{
			_idCopyBuffer.clear();
		
			for (const TrackedObject& object : _trackedObjects) 
			{
				// store ids of all grids on current frame in copy buffer
				if (object.count(_currentFrameNumber)) 
				{
					_idCopyBuffer.insert(object.getId());
				}
			}
			_copyFromFrame = _currentFrameNumber;
		}
		else 
			if (e->key() == Qt::Key_V && e->modifiers().testFlag(Qt::ControlModifier) && _copyFromFrame) 
			{
				for (TrackedObject& object : _trackedObjects) 
				{
					// check if id of object is in copy buffer
					if (_idCopyBuffer.count(object.getId())) 
					{
						// check if grid from copy-from framenumber still exists
						const auto maybeGrid = object.maybeGet<Grid3D>(_copyFromFrame.get());
						// and create a copy if a grid with the same id does not
						// already exist on the current frame
						if (maybeGrid && !object.maybeGet<Grid3D>(_currentFrameNumber))
						{
							object.add(_currentFrameNumber, std::make_shared<Grid3D>(*maybeGrid));
						}
					}
				}

				emit update();

				setNumTags();
			}
			else 
			{
				switch (e->key())
				{
				case Qt::Key_H:
					_activeGrid->setYRotation(_activeGrid->getYRotation() + 0.05);
					break;
				case Qt::Key_G:
					_activeGrid->setYRotation(_activeGrid->getYRotation() - 0.05);
					break;
				case Qt::Key_W:
					_activeGrid->setXRotation(_activeGrid->getXRotation() - 0.05);
					break;
				case Qt::Key_S:
					_activeGrid->setXRotation(_activeGrid->getXRotation() + 0.05);
					break;
				case Qt::Key_A:
					_activeGrid->setZRotation(_activeGrid->getZRotation() - 0.05);
					break;
				case Qt::Key_D:
					_activeGrid->setZRotation(_activeGrid->getZRotation() + 0.05);
					break;
				case Qt::Key::Key_CapsLock:
					_activeGrid->toggleTransparency();
					break;
				default:
					return;
				}
			emit update();
	}
}

//BeesBookTagMatcher private member functions

//DRAWING FUNCTIONS

//function that draws the set Tags so far.
void BeesBookTagMatcher::drawSetTags(cv::Mat& image) const
{
	// iterate over all stored object
	for ( const TrackedObject& trackedObject : _trackedObjects )
	{
		// check: data for that frame exists
		if ( trackedObject.count( _currentFrameNumber ) )
		{
			// get grid
			std::shared_ptr<Grid3D> grid = trackedObject.get<Grid3D>(_currentFrameNumber);

			// do not paint grid if it is active (will be drawn with different color later)
			if (grid != _activeGrid)
			{
				grid->draw(image);
			}
		}
	}
}

//function that draws the orientation vector while being set.
void BeesBookTagMatcher::drawOrientation(cv::Mat &image, const Orientation &orient) const
{
	//the orientation vector is printed in red
	cv::line(image, orient.from, orient.to, cv::Scalar(0, 0, 255), 1);
}

//function that draws an active tag calling an instance of Grid3D
void BeesBookTagMatcher::drawActiveTag(cv::Mat &image) const
{
	// ToDo: different color for active tag?
	_activeGrid->draw(image);         //the grid is drawn as active
	//_activeGrid->updatePoints();
	//for (int i = 0; i < 3; i++)
	//{
	//	cv::circle(image, _activeGrid->absPoints[i], 1, COLOR_RED, 1);                 //the point is drawn in red
	//}
	////active point in blue

	//switch (_currentState) {
	//case State::SetP0:
	//	cv::circle(image, _activeGrid->absPoints[0], 1, COLOR_BLUE, 1);
	//	break;
	//case State::SetP1:
	//	cv::circle(image, _activeGrid->absPoints[1], 1, COLOR_BLUE, 1);
	//	break;
	//case State::SetP2:
	//	cv::circle(image, _activeGrid->absPoints[2], 1, COLOR_BLUE, 1);
	//	break;
	//default:
	//	break;
	//}
}

//function that draws the tag while being rotated in space
void BeesBookTagMatcher::drawSettingTheta(cv::Mat &img) const
{
	//cv::ellipse(img, _activeGrid->absPoints[0], _activeGrid->axesTag, _activeGrid->angleTag, 0, 360, COLOR_YELLOW, 1);
	//cv::line(img, _activeGrid->absPoints[0], _activeGrid->absPoints[0] + _activeGrid->realCoord[1], COLOR_RED, 1);          //the orientation vector is printed in red
	//cv::line(img, _activeGrid->absPoints[0], _activeGrid->absPoints[0] + _activeGrid->realCoord[2], COLOR_GREEN, 1);        //the redius vector is printed in green

	//for (int i = 0; i < 3; i++)
	//{
	//	cv::circle(img, _activeGrid->absPoints[0] + _activeGrid->realCoord[i], 1, COLOR_RED, 1);               //the point is drawn in red
	//}
	//switch (_currentState) {
	//case State::SetP1:
	//	cv::circle(img, _activeGrid->absPoints[0] + _activeGrid->realCoord[1], 1, COLOR_BLUE, 1);
	//	break;
	//case State::SetP2:
	//	cv::circle(img, _activeGrid->absPoints[0] + _activeGrid->realCoord[2], 1, COLOR_BLUE, 1);
	//	break;
	//default:
	//	break;
	//}
}

//TAG CONFIGURATION FUNCTIONS

//function called while setting the tag (it initializes orient vector)
void BeesBookTagMatcher::setTag(const cv::Point& location) 
{
	_currentState = State::SetTag;

	// first point of line
	_orient.from = location;
	// second point of line (will be updated when mouse is moved)
	_orient.to   = location;

	//the orientation vector is drawn.
	emit update();
}

//function that allows P1 and P2 to be modified to calculate the tag's angle in space.
void BeesBookTagMatcher::setTheta(const cv::Point &location) 
{
	//double prop;
	//double angle;

	//switch (_currentState) {
	//case State::SetP1:
	//	//the length of the vector is equal to the distance to the pointer, limited to axisTag and 0.5*axisTag
	//	prop = dist(_activeGrid->absPoints[0], location) / (_activeGrid->scale * BeesBookTag::AXISTAG);
	//	if (prop > 1)
	//		prop = 1;
	//	else if (prop < 0.5)
	//		prop = 0.5;
	//	//P1 updates the orientation of the tag too.
	//	_activeGrid->orientation(location);
	//	_activeGrid->realCoord[1] = _activeGrid->polar2rect(prop * BeesBookTag::AXISTAG, _activeGrid->alpha);
	//	break;
	//case State::SetP2:
	//	//the length of the vector is equal to the distance to the pointer, limited to axisTag and 0.5*axisTag
	//	prop = dist(_activeGrid->absPoints[0], location) / (_activeGrid->scale * BeesBookTag::AXISTAG);
	//	if (prop > 1)
	//		prop = 1;
	//	else if (prop < 0.5)
	//		prop = 0.5;
	//	//P2 doesn't update alpha
	//	angle = atan2(location.x - _activeGrid->centerGrid.x, location.y - _activeGrid->centerGrid.y) - CV_PI / 2;
	//	_activeGrid->realCoord[2] = _activeGrid->polar2rect(prop * BeesBookTag::AXISTAG, angle);
	//	break;
	//default:
	//	break;
	//}
	////updates parameters
	//_activeGrid->updateParam();
	return;
}

//function that checks if one of the set Points is selected, returns true when one of the points is selected
bool BeesBookTagMatcher::selectPoint(const cv::Point& location) {
	//for (int i = 0; i < 3; i++) {
	//	if (dist(location, _activeGrid->absPoints[i]) < 2) //check if the pointer is on one of the points
	//	{
	//		switch (i)
	//		{
	//		case 0:
	//			_currentState = State::SetP0;
	//			return true;
	//		case 1:
	//			_currentState = State::SetP1;
	//			return true;
	//		case 2:
	//			_currentState = State::SetP2;
	//			return true;
	//		}
	//	}
	//}
	return false;
}

//function that checks if one of the already set Tags is selected.
void BeesBookTagMatcher::selectTag(const cv::Point& location)
{
	// iterate over all stored objects
	for (size_t i = 0; i < _trackedObjects.size(); i++)
	{
		// get pointer to i-th object
		std::shared_ptr<Grid3D> grid = _trackedObjects[i].maybeGet<Grid3D>(_currentFrameNumber);

		// check if grid is valid
		if (grid && dist(location, grid->getCenter()) < grid->getPixelRadius())
		{
			// assign the found grid to the activegrid pointer
			_activeGrid        = grid;
			_activeFrameNumber = _currentFrameNumber;

			emit update();

			return;
		}
	}
}

void BeesBookTagMatcher::cancelTag()
{
	_activeGrid.reset();
	_activeFrameNumber.reset();
	_currentState  = State::Ready;
	_setOnlyOrient = false;
}

void BeesBookTagMatcher::removeCurrentActiveTag()
{	
	//if (_activeGrid)
	//{
	//	auto trackedObjectIterator = std::find_if(_trackedObjects.begin(), _trackedObjects.end(),
	//	                                          [&](const TrackedObject & o){ return o.getId() == _activeGrid->objectId; }) ;
	//	
	//	assert( trackedObjectIterator != _trackedObjects.end() );

	//	trackedObjectIterator->erase(_currentFrameNumber);

	//	// if map empty
	//	if (trackedObjectIterator->isEmpty())
	//	{
	//		// delete from _trackedObjects
	//		_trackedObjects.erase(trackedObjectIterator);
	//	}
	//
	//	// reset active tag and frame and...
	//	cancelTag();

	//	setNumTags();
	//}
}

//AUXILIAR FUNCTION

//function that calculates the distance between two points
double BeesBookTagMatcher::dist(const cv::Point &p1, const cv::Point &p2) const
{
	const cv::Point diff = p1 - p2;
	return sqrt(diff.x * diff.x + diff.y * diff.y);
}

void BeesBookTagMatcher::setNumTags()
{
	size_t cnt = 0;
	for (size_t i = 0; i < _trackedObjects.size(); i++)
	{
		if (_trackedObjects[i].maybeGet<Grid3D>(_currentFrameNumber)) {
			++cnt;
		}
	}

	_UiToolWidget.numTags->setText(QString::number(cnt));
}

double BeesBookTagMatcher::getRadiusFromFocalLength() const
{
	// ToDo
	return 52.;
}

const std::set<Qt::Key> &BeesBookTagMatcher::grabbedKeys() const
{
	static const std::set<Qt::Key> keys { Qt::Key_Plus, Qt::Key_Minus,
	                                      Qt::Key_C, Qt::Key_V,
										  Qt::Key_W, Qt::Key_A,
										  Qt::Key_S, Qt::Key_D,
										  Qt::Key_G, Qt::Key_H, Qt::Key_CapsLock };
	return keys;
}

bool BeesBookTagMatcher::event(QEvent *event)
{
	const QEvent::Type etype = event->type();
	switch (etype) {
	case QEvent::KeyPress:
		keyPressEvent(static_cast<QKeyEvent*>(event));
		return true;
		break;
	case QEvent::MouseButtonPress:
		mousePressEvent(static_cast<QMouseEvent*>(event));
		return true;
		break;
	case QEvent::MouseButtonRelease:
		mouseReleaseEvent(static_cast<QMouseEvent*>(event));
		return true;
		break;
	case QEvent::MouseMove:
		mouseMoveEvent(static_cast<QMouseEvent*>(event));
		return true;
		break;
	default:
		event->ignore();
		return false;
	}
}
