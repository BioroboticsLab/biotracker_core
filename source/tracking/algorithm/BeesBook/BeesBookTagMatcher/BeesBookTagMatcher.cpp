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

//static const cv::Scalar COLOR_BLUE   = cv::Scalar(255, 0, 0);
static const cv::Scalar COLOR_RED    = cv::Scalar(0, 0, 255);
static const cv::Scalar COLOR_GREEN  = cv::Scalar(0, 255, 0);
static const cv::Scalar COLOR_YELLOW = cv::Scalar(0, 255, 255);
static const cv::Scalar COLOR_ORANGE = cv::Scalar(0, 102, 255);
}

const size_t BeesBookTagMatcher::GRID_RADIUS_PIXELS = 26;

BeesBookTagMatcher::BeesBookTagMatcher(Settings & settings, QWidget *parent)
	: TrackingAlgorithm(settings, parent)
	, _currentState(State::Ready)
	, _lastMouseEventTime(std::chrono::system_clock::now())
	, _toolWidget(std::make_shared<QWidget>())
	, _paramWidget(std::make_shared<QWidget>())
	, _visualizeFrames(true)
{
	_UiToolWidget.setupUi(_toolWidget.get());
	setNumTags();
}

BeesBookTagMatcher::~BeesBookTagMatcher()
{}

void BeesBookTagMatcher::track(ulong /* frameNumber */, cv::Mat & img/* frame */)
{
    _imgRect = cv::Rect( cv::Point(0,0), img.size() );
    updateValidRect();
	resetActiveGrid();
	setNumTags();
}

void BeesBookTagMatcher::paint(cv::Mat& image, const View&)
{
	if (!_trackedObjects.empty() || _activeGrid)
	{
		drawTags(image);
	}
	if (_currentState == State::SetTag)
	{
		drawOrientation(image, _orient);
	}
}

/**
    called after loading serialization data, see TrackingAlgorithm.h for declaration
*/
void BeesBookTagMatcher::postLoad()
{
	setNumTags();
}

// called when MOUSE BUTTON IS CLICKED
void BeesBookTagMatcher::mousePressEvent(QMouseEvent * e)
{
    bool dataChanged = false;

    // position of mouse cursor 
    cv::Point mousePosition(e->x(), e->y());

    // restrict mouse pointer to live inside image borders only
    if (!mousePosition.inside(cv::Rect( _imgRect)) )
        forcePointIntoBorders(mousePosition, _imgRect);

    // keyboard modifiers
    const bool ctrlModifier     = QApplication::keyboardModifiers().testFlag(Qt::ControlModifier);
    const bool shiftModifier    = QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier);

    if (e->button() == Qt::LeftButton)
    {

        // LMB +  Ctrl
        if (ctrlModifier & !shiftModifier)		
        {
            // reset pointer
            resetActiveGrid();

            // force mouse pointer to be inside valid boundaries
            forcePointIntoBorders(mousePosition, _validRect);
            
            // initialize new orientation vector, ie start drawing a line
            setTag(mousePosition);
            
            // data has changed: update!
            dataChanged = true;
        }
        else // other LMB
        {
            if (_activeGrid)
            {
                // find id of keypoint clicked with the mouse
                int id = _activeGrid->getKeyPointIndex(mousePosition);

                // CTRL + Shift --> set bit to indeterminate
                const bool indeterminate = ctrlModifier && shiftModifier;

                // one of the twelve cells is clicked
                if ((id >= 0) && (id < 12)) // ToDo: use constants
                {
                    // change state, so consequent mouse moves do not move the tag when toggling bits
                    _currentState = State::SetBit;

                    // toggle bit or set indeterminate, resp.
                    _activeGrid->toggleIdBit(id, indeterminate);

                    // data has changed: update!
                    dataChanged = true;
                }
                else // another keypoint was clicked
                {
                    switch (id)
                    {
                    case 12: // center point
                    {
                        _currentState = State::SetP0;
                        break;
                    }
                    case 13: // P1 (the one to rotate in x/y-plane), ToDo: use constants
                    {
                        _currentState = State::SetP1;
                        break;
                    }
                    default: // no keypoint: select another tag
                    {
                        selectTag(mousePosition);
                    }
                    }
                }
            }
            else // LMB and no active grid
            {
                selectTag(mousePosition);
            }
        }
    }
    // RMB
    else if (e->button() == Qt::RightButton)
    {
        if (_activeGrid)
        {
            if (ctrlModifier) 
            {
                // if mouse cursor roughly inside tag
                if (dist(mousePosition, _activeGrid->getCenter()) < _activeGrid->getPixelRadius())
                {
                    removeCurrentActiveTag();
                    
                    // data has changed: update!
                    dataChanged = true;
                }
            }
            else // RMB + !ctrl
            {
                // if close to tag: rotate in space
                if (dist(mousePosition, _activeGrid->getCenter()) < 2 * _activeGrid->getPixelRadius())
                {
                    // vector orthogonal to rotation axis
                    _tempPoint = mousePosition - _activeGrid->getCenter();
                   
                    // set "rotation in space"-state
                    _currentState = State::SetP2;
                }
            }
        }
    }

    if (dataChanged)
        emit update();
}

// called when mouse pointer MOVES
void BeesBookTagMatcher::mouseMoveEvent(QMouseEvent * e) 
{
    cv::Point mousePosition(e->x(), e->y());

    // position of mouse cursor
    if (!mousePosition.inside(cv::Rect(_imgRect)))
        forcePointIntoBorders(mousePosition, _imgRect);

    // get current time
	const auto elapsed = std::chrono::system_clock::now() - _lastMouseEventTime;
	
    // slow down update rate to 1000 Hz max
    if (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() > 1) 
    {
		switch (_currentState) 
		{
        case State::SetTag: // new tag is being drawn : update tip of orientation vector
        {
            _orient.to = mousePosition;
            break;
        }
		case State::SetP0:  // tag is being moved 
		{
            forcePointIntoBorders(mousePosition, _validRect);
            _activeGrid->setCenter(mousePosition);
			break;
		}
		case State::SetP1: // tag is rotated in grid-plane
		{
            _activeGrid->zRotateTowardsPointInPlane(mousePosition);
            this->updateValidRect();
			break;
		}		
		case State::SetP2:  // tag is rotated in space
		{
            // distance moved since first click or last move event
            float d = static_cast<float>(cv::norm(_tempPoint) - cv::norm(mousePosition - _activeGrid->getCenter()));
            
            // vector orthogonal to rotation axis
            _tempPoint = mousePosition - _activeGrid->getCenter();

			// distance of mouse cursor to center
            const float d1          = static_cast<float>(cv::norm(_tempPoint));

            // skip this situation (avoid division by zero)
            if (d1 == 0)
                break;

			// the rotation axis in image reference frame (unit vector)
            const float x           = -_tempPoint.y / d1;
            const float y           = _tempPoint.x  / d1;

			// z - angle of grid
			const double a          = _activeGrid->getZRotation();

			// the rotation axis in grid reference frame (ToDo: rotate in space?)
			_rotationAxis.x         = static_cast<float>(cos(a) * x + sin(a) * y);
			_rotationAxis.y         = static_cast<float>(-sin(a) * x + cos(a) * y);
						
			// weight of rotation
			const float w = 0.05f * d;
			
            // apply rotation
            _activeGrid->xyRotateIntoPlane(w * _rotationAxis.y + static_cast<float>(_activeGrid->getYRotation()),
			                               w * _rotationAxis.x + static_cast<float>(_activeGrid->getXRotation()));
            this->updateValidRect();
			break;
		}
		default: // other states (like State::SetBit or ::Ready) 
			return;
		}

		emit update();
		
        _lastMouseEventTime = std::chrono::system_clock::now();
	}
}

// called when MOUSE BUTTON IS RELEASED
void BeesBookTagMatcher::mouseReleaseEvent(QMouseEvent * e) 
{
    bool dataChanged = false;

	// left button released
	if (e->button() == Qt::LeftButton) 
    {
		switch (_currentState) 
        {
		// a new tag is being created
		case State::SetTag:
		{
			// update active frame number and active grid
			_activeFrameNumber = getCurrentFrameNumber();

            // generate object id
			const size_t newID = _trackedObjects.empty() ? 0 : _trackedObjects.back().getId() + 1;

			// update active frame number, objectId and grid
			_activeFrameNumber = getCurrentFrameNumber();
			_activeGridObjectId = newID;

			// insert new trackedObject object into _trackedObjects ( check if empty "first")
			_trackedObjects.emplace_back(newID);

			// make pointer to the new tag
            _activeGrid = std::make_shared<Grid3D>(_orient.from, static_cast<double>(GRID_RADIUS_PIXELS), _orient.alpha(), 0., 0.);
            
            // associate new (active) grid to frame number
            _trackedObjects.back().add(getCurrentFrameNumber(), _activeGrid);

            // update GUI display 
			setNumTags();

			this->updateValidRect();

            // data has changed, thus emit update in end of function
            dataChanged = true;

			break;
		}
        default:
			break;
		}      
	}

    // switch to ready-state when mouse is released
    _currentState = State::Ready;

    if (dataChanged)
        emit update();
}

void BeesBookTagMatcher::keyPressEvent(QKeyEvent *e)
{
	// general key events
	// -------------------

	// toggle frames around tags
	if (e->key() == Qt::Key_F)
	{
		_visualizeFrames = !_visualizeFrames;
	}

	// CTRL + C
	else if (e->modifiers().testFlag(Qt::ControlModifier) && e->key() == Qt::Key_C)
	{
		this->copyTrackedObjects();
	}

	// CTRL + V
	else if (e->modifiers().testFlag(Qt::ControlModifier) && e->key() == Qt::Key_V)
	{
		this->pasteTrackedObjects();
	}

	// modify active grid events
	// --------------------------
	else if (_activeGrid) {

		static const double rotateIncrement = 0.05;

		switch (e->key()) {

			// change radius
			case Qt::Key_Plus:
			case Qt::Key_Minus: {
				const double direction = e->key() == Qt::Key_Plus ? 1.f : -1.f;
				const double radius = _activeGrid->getWorldRadius();
				_activeGrid->setWorldRadius(radius + direction * 0.01 * radius);
				updateValidRect();
				break;
			}

			// toggle transparency
			case Qt::Key::Key_CapsLock:
				_activeGrid->toggleTransparency();
				break;

			// rotate
			case Qt::Key_H:
				_activeGrid->setYRotation(_activeGrid->getYRotation() + rotateIncrement);
				this->updateValidRect();
				break;
			case Qt::Key_G:
				_activeGrid->setYRotation(_activeGrid->getYRotation() - rotateIncrement);
				this->updateValidRect();
				break;
			case Qt::Key_W:
				_activeGrid->setXRotation(_activeGrid->getXRotation() - rotateIncrement);
				this->updateValidRect();
				break;
			case Qt::Key_S:
				_activeGrid->setXRotation(_activeGrid->getXRotation() + rotateIncrement);
				this->updateValidRect();
				break;
			case Qt::Key_A:
				_activeGrid->setZRotation(_activeGrid->getZRotation() - rotateIncrement);
				this->updateValidRect();
				break;
			case Qt::Key_D:
				_activeGrid->setZRotation(_activeGrid->getZRotation() + rotateIncrement);
				this->updateValidRect();
				break;
			case Qt::Key_U:
				_activeGrid->setSettable(!_activeGrid->isSettable());
				break;

			// change transsparency:
			// 0 -->  0% transparent
			// 1 --> 10% transparent
			// ...
			// 9 --> 90% transparent
			case Qt::Key_0:
				_activeGrid->setTransparency(1.0f - 0.0f);
				break;
			case Qt::Key_1:
				_activeGrid->setTransparency(1.0f - 0.1f);
				break;
			case Qt::Key_2:
				_activeGrid->setTransparency(1.0f - 0.2f);
				break;
			case Qt::Key_3:
				_activeGrid->setTransparency(1.0f - 0.3f);
				break;
			case Qt::Key_4:
				_activeGrid->setTransparency(1.0f - 0.4f);
				break;
			case Qt::Key_5:
				_activeGrid->setTransparency(1.0f - 0.5f);
				break;
			case Qt::Key_6:
				_activeGrid->setTransparency(1.0f - 0.6f);
				break;
			case Qt::Key_7:
				_activeGrid->setTransparency(1.0f - 0.7f);
				break;
			case Qt::Key_8:
				_activeGrid->setTransparency(1.0f - 0.8f);
				break;
			case Qt::Key_9:
				_activeGrid->setTransparency(1.0f - 0.9f);
				break;

			default:
				break;
		} // END: switch (e->key())
	} // END: _activeGrid

	// TODO: skip "emit update()" if event doesn't alter image (i.e. ctrl + 0)
	emit update();
}

void BeesBookTagMatcher::copyTrackedObjects() {
	_idCopyBuffer.clear();

	for (const TrackedObject& object : _trackedObjects)
	{
		// store ids of all grids on current frame in copy buffer
		if (object.count(getCurrentFrameNumber()))
		{
			_idCopyBuffer.insert(object.getId());
		}
	}
	_copyFromFrame = getCurrentFrameNumber();
}


void BeesBookTagMatcher::pasteTrackedObjects() {
	if (_copyFromFrame)
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
				if (maybeGrid && !object.maybeGet<Grid3D>(getCurrentFrameNumber()))
				{
					// set toogled state to indeterminate if the grid has been set before
					const auto newGrid = std::make_shared<Grid3D>(*maybeGrid);
					if (newGrid->hasBeenBitToggled().value == boost::logic::tribool::value_t::true_value) {
						newGrid->setBeenBitToggled(boost::logic::tribool::value_t::indeterminate_value);
					}
					object.add(getCurrentFrameNumber(), newGrid);
				}
			}
		}
		setNumTags();
	}
}


//function that draws the set Tags so far.
void BeesBookTagMatcher::drawTags(cv::Mat& image) const
{
	// iterate over all stored object
	for ( const TrackedObject& trackedObject : _trackedObjects )
	{
		// check: data for that frame exists
		if ( trackedObject.count( getCurrentFrameNumber() ) )
		{
			// get grid
			const std::shared_ptr<Grid3D> grid = trackedObject.get<Grid3D>(getCurrentFrameNumber());
			const bool isActive = grid == _activeGrid;

			grid->draw(image, isActive);

			if (_visualizeFrames) {
				// calculate actual pixel size of grid based on current zoom level
				const double displayTagSize = std::min(grid->getPixelRadius() / getCurrentZoomLevel(), 50.);
				// thickness of rectangle of grid is based on actual pixel size
				// of the grid. if the radius is 50px or more, the rectangle has
				// a thickness of 1px.
				const int thickness = static_cast<int>(1. / (displayTagSize / 50.));

				// draw rectangle around grid
				const cv::Point center = grid->getCenter();
				const int radius       = static_cast<int>(grid->getPixelRadius() * 1.5);
				const cv::Point tl(center.x - radius, center.y - radius);
				const cv::Point br(center.x + radius, center.y + radius);
				const cv::Scalar color = getGridColor(grid);
				cv::rectangle(image, tl, br, color, thickness, CV_AA);

				// draw tracked object id
				const cv::Point bl(center.x + radius, center.y - radius);
				const auto id_str = std::to_string(trackedObject.getId());
				cv::putText(image, id_str, bl, cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, color);
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

//function called once when creating a new tag (it initializes orient vector)
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

//function that checks if one of the already set Tags is selected.
void BeesBookTagMatcher::selectTag(const cv::Point& location)
{
	// iterate over all stored objects
	for (size_t i = 0; i < _trackedObjects.size(); i++)
	{
		// get pointer to i-th object
		std::shared_ptr<Grid3D> grid = _trackedObjects[i].maybeGet<Grid3D>(getCurrentFrameNumber());

		// check if grid is valid
		if (grid && dist(location, grid->getCenter()) < grid->getPixelRadius())
		{
			// assign the found grid to the activegrid pointer
			_activeGrid         = grid;
			_activeFrameNumber  = getCurrentFrameNumber();
			_activeGridObjectId = _trackedObjects[i].getId();

			// if tag state is set to indeterminate, set it to true again
			if (_activeGrid->hasBeenBitToggled().value == boost::logic::tribool::indeterminate_value)
				_activeGrid->setBeenBitToggled(boost::logic::tribool::true_value);

			emit update();

			return;
		}
	}
}

void BeesBookTagMatcher::cancelTag()
{
	resetActiveGrid();
	_currentState  = State::Ready;
}

void BeesBookTagMatcher::resetActiveGrid()
{
	_activeGrid.reset();
	_activeFrameNumber.reset();
	_activeGridObjectId.reset();
}

void BeesBookTagMatcher::removeCurrentActiveTag()
{
	assert(_activeGrid);

	auto trackedObjectIterator = std::find_if(_trackedObjects.begin(), _trackedObjects.end(),
											  [&](const TrackedObject & o){ return o.getId() == _activeGridObjectId.get(); }) ;

	assert(trackedObjectIterator != _trackedObjects.end());

	trackedObjectIterator->erase(getCurrentFrameNumber());

	// if map empty
	if (trackedObjectIterator->isEmpty())
	{
		// delete from _trackedObjects
		_trackedObjects.erase(trackedObjectIterator);
	}

	// reset active tag and frame and...
	cancelTag();

	setNumTags();
}

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
		if (_trackedObjects[i].maybeGet<Grid3D>(getCurrentFrameNumber())) {
			++cnt;
		}
	}

	_UiToolWidget.numTags->setText(QString::number(cnt));
}

const std::set<Qt::Key> &BeesBookTagMatcher::grabbedKeys() const
{
	static const std::set<Qt::Key> keys { Qt::Key_Plus, Qt::Key_Minus,
	                                      Qt::Key_C, Qt::Key_V,
	                                      Qt::Key_W, Qt::Key_A,
	                                      Qt::Key_S, Qt::Key_D,
	                                      Qt::Key_G, Qt::Key_H,
	                                      Qt::Key_U, Qt::Key_F,
	                                      Qt::Key_CapsLock,
	                                      Qt::Key_0, Qt::Key_1, Qt::Key_2, Qt::Key_3,
	                                      Qt::Key_4, Qt::Key_5, Qt::Key_6, Qt::Key_7,
	                                      Qt::Key_8, Qt::Key_9
	                                    };
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

void BeesBookTagMatcher::forcePointIntoBorders(cv::Point & point, cv::Rect const & borders)
{
    if (point.x < borders.x)
        point.x = borders.x;
    else if (point.x >= ( borders.x + borders.width) )
        point.x = (borders.x + borders.width - 1);

    if (point.y < borders.y)
        point.y = borders.y;
    else if (point.y >= ( borders.y + borders.height) )
        point.y = ( borders.y + borders.height - 1 );
}

void BeesBookTagMatcher::updateValidRect()
{
	if (_activeGrid)
	{
		const auto box = _activeGrid->getOriginBoundingBox();
		_validRect = cv::Rect(cv::Point2i(0, 0) - box.tl(), cv::Point2i(_imgRect.width, _imgRect.height) - box.br());
	}
	else {
		_validRect = cv::Rect(_imgRect.x + GRID_RADIUS_PIXELS,
		                      _imgRect.y + GRID_RADIUS_PIXELS,
		                      _imgRect.width - 2 * GRID_RADIUS_PIXELS,
		                      _imgRect.height - 2 * GRID_RADIUS_PIXELS);
	}
}

cv::Scalar BeesBookTagMatcher::getGridColor(const std::shared_ptr<Grid3D> &grid) const
{
	if (grid->isSettable()) {
		switch (grid->hasBeenBitToggled().value) {
		case boost::logic::tribool::value_t::true_value:
			return COLOR_GREEN;
		case boost::logic::tribool::value_t::false_value:
			return COLOR_YELLOW;
		case boost::logic::tribool::value_t::indeterminate_value:
			return COLOR_ORANGE;
		default:
			assert(false);
			return COLOR_RED;
		}
	} else {
		return COLOR_RED;
	}
}
