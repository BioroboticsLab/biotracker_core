#include "TrackingAlgorithm.h"

TrackingAlgorithm::TrackingAlgorithm(Settings & settings, QWidget *parent) 
	: _settings(settings),
	_parent(parent)
{
}

TrackingAlgorithm::~TrackingAlgorithm() {}

void TrackingAlgorithm::loadObjects( std::vector<TrackedObject> * objects )
{	_trackedObjects = objects;	}
void TrackingAlgorithm::mouseMoveEvent		( QMouseEvent * )	{}
void TrackingAlgorithm::mousePressEvent		( QMouseEvent * )	{}
void TrackingAlgorithm::mouseReleaseEvent	( QMouseEvent * )	{}
void TrackingAlgorithm::mouseWheelEvent		( QWheelEvent * )	{}
QWidget* TrackingAlgorithm::getToolsWidget	()	{ return  new QWidget(_parent,0); }
QWidget* TrackingAlgorithm::getParamsWidget	()	{ return  new QWidget(_parent,0); }