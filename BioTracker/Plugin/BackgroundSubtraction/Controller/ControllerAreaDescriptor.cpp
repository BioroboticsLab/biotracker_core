#include "ControllerAreaDescriptor.h"
#include "ControllerTrackedComponent.h"
#include "ControllerTrackingAlgorithm.h"

#include "Model/AreaInfo.h"
#include "View/AreaDescriptorView.h"
#include "View/AreaDesciptor/AreaDescriptor.h"
#include "View/AreaDesciptor/RectDescriptor.h"
#include "View/AreaDesciptor/EllipseDescriptor.h"

#include "View/TrackedElementView.h"
#include "View/TrackerParameterView.h"

ControllerAreaDescriptor::ControllerAreaDescriptor(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
	IController(parent, context, ctr)
{

}

void ControllerAreaDescriptor::createView()
{
	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COMPONENT);

	AreaDescriptorView* view = new AreaDescriptorView(dynamic_cast<TrackedElementView*>(qobject_cast<ControllerTrackedComponent*>(ctr)->getView()), this, 0);
	AreaInfo* area = dynamic_cast<AreaInfo*>(getModel());

	m_View = view;

	view->_rectif = new RectDescriptor(view, this, area->_rect.get());
	QObject::connect(view->_rectif, &RectDescriptor::updatedPoints, view, &AreaDescriptorView::updateRectification);

	BioTracker::Core::Settings *_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CONFIGPARAM::CONFIG_INI_FILE);
	int v = _settings->getValueOrDefault<int>(TRACKERPARAM::CN_APPERTURE_TYPE, 0);
	view->_tracking = new RectDescriptor(view, this, area->_apperture.get());
	trackingAreaType(v);

	//view->_tracking = new RectDescriptor(view, this, area->_apperture.get());
	//area->_apperture->_type = 0;
	//view->_tracking->setBrush(QBrush(Qt::red));
	//QObject::connect(view->_tracking, &RectDescriptor::updatedPoints, view, &AreaDescriptorView::updateApperture);

}

void ControllerAreaDescriptor::connectModelToController()
{

}

void ControllerAreaDescriptor::trackingAreaType(int v) {

	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COMPONENT);
	AreaDescriptorView* view = dynamic_cast<AreaDescriptorView*>(m_View);
	AreaInfo* area = dynamic_cast<AreaInfo*>(getModel());
	if (view->_tracking) {
		delete view->_tracking;
	}

	BioTracker::Core::Settings *_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CONFIGPARAM::CONFIG_INI_FILE);
	if (v == 0) {
		view->_tracking = new RectDescriptor(view, this, area->_apperture.get());
		area->_apperture->_type = 0;
		view->_tracking->setBrush(QBrush(Qt::red));
		QObject::connect(view->_tracking, &RectDescriptor::updatedPoints, view, &AreaDescriptorView::updateApperture);
		_settings->setParam(TRACKERPARAM::CN_APPERTURE_TYPE, 0);
	}
	else if (v > 0) {
		view->_tracking = new EllipseDescriptor(view, this, area->_apperture.get());
		area->_apperture->_type = 1;
		view->_tracking->setBrush(QBrush(Qt::red));
		QObject::connect(view->_tracking, &EllipseDescriptor::updatedPoints, view, &AreaDescriptorView::updateApperture);
		_settings->setParam(TRACKERPARAM::CN_APPERTURE_TYPE, 1);
	}
}

void ControllerAreaDescriptor::connectControllerToController()
{
	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COMPONENT);
	AreaDescriptorView* view = new AreaDescriptorView(dynamic_cast<TrackedElementView*>(qobject_cast<ControllerTrackedComponent*>(ctr)->getView()), this, 0);

	IController* ctrTr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TRACKING);
	TrackerParameterView * pv = dynamic_cast<TrackerParameterView*>(qobject_cast<ControllerTrackingAlgorithm*>(ctrTr)->getView());
	QObject::connect(pv, &TrackerParameterView::trackingAreaType, this, &ControllerAreaDescriptor::trackingAreaType);
}

void ControllerAreaDescriptor::createModel()
{
	m_Model = new AreaInfo();
}
