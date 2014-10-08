#include "TrackingAlgorithm.h"
#include "source/helper/StringHelper.h"

TrackingAlgorithm::TrackingAlgorithm(Settings & settings, QWidget *parent) 
	: _settings(settings),
	_parent(parent)
{
}

TrackingAlgorithm::~TrackingAlgorithm() {}

void TrackingAlgorithm::loadObjects( std::vector<TrackedObject> * objects )
{	_trackedObjects = objects;	}
void TrackingAlgorithm::mouseMoveEvent		( QMouseEvent * e )	{}
void TrackingAlgorithm::mousePressEvent		( QMouseEvent * e )	{}
void TrackingAlgorithm::mouseReleaseEvent	( QMouseEvent * e )	{}
void TrackingAlgorithm::mouseWheelEvent		( QWheelEvent * e )	{}
QWidget* TrackingAlgorithm::getToolsWidget	()	{ return  new QWidget(_parent,0); }
QWidget* TrackingAlgorithm::getParamsWidget	()	{ return  new QWidget(_parent,0); }