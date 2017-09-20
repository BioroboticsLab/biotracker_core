#include "ControllerAreaDescriptor.h"

#include "Model/AreaDescriptor/AreaInfo.h"
#include "View/AreaDescriptorView.h"
#include "View/AreaDesciptor/AreaDescriptor.h"
#include "View/AreaDesciptor/RectDescriptor.h"
#include "View/AreaDesciptor/EllipseDescriptor.h"
#include "settings/Settings.h"
#include "util/types.h"

#include "ControllerGraphicScene.h"
#include "View/GraphicsView.h"

ControllerAreaDescriptor::ControllerAreaDescriptor(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
	IController(parent, context, ctr)
{

}

void ControllerAreaDescriptor::createView()
{
	//IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COMPONENT);

	assert(m_Model);
	AreaDescriptorView* view = new AreaDescriptorView(this, m_Model);
	m_View = view;

	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);
	auto viewController = qobject_cast<ControllerGraphicScene*>(ctr);
	auto gview = dynamic_cast<GraphicsView*> (viewController->getView());
	gview->addGraphicsItem(view);

	AreaInfo* area = dynamic_cast<AreaInfo*>(getModel());
	
	view->_rectif = new RectDescriptor(view, this, area->_rect.get());
	QObject::connect(view->_rectif, &RectDescriptor::updatedPoints, view, &AreaDescriptorView::updateRectification);

	BioTracker::Core::Settings *_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
	int v = _settings->getValueOrDefault<int>(AREADESCRIPTOR::CN_APPERTURE_TYPE, 0);
	view->_tracking = new RectDescriptor(view, this, area->_apperture.get());
	trackingAreaType(v);
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

	BioTracker::Core::Settings *_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
	if (v == 0) {
		view->_tracking = new RectDescriptor(view, this, area->_apperture.get());
		area->_apperture->_type = 0;
		view->_tracking->setBrush(QBrush(Qt::red));
		QObject::connect(view->_tracking, &RectDescriptor::updatedPoints, view, &AreaDescriptorView::updateApperture);
		_settings->setParam(AREADESCRIPTOR::CN_APPERTURE_TYPE, 0);
	}
	else if (v > 0) {
		view->_tracking = new EllipseDescriptor(view, this, area->_apperture.get());
		area->_apperture->_type = 1;
		view->_tracking->setBrush(QBrush(Qt::red));
		QObject::connect(view->_tracking, &EllipseDescriptor::updatedPoints, view, &AreaDescriptorView::updateApperture);
		_settings->setParam(AREADESCRIPTOR::CN_APPERTURE_TYPE, 1);
	}
}

void ControllerAreaDescriptor::connectControllerToController()
{
	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COMPONENT);
	AreaDescriptorView* view = new AreaDescriptorView(this, m_Model);

	IController* ctrTr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TRACKING);
	//TrackerParameterView * pv = dynamic_cast<TrackerParameterView*>(qobject_cast<ControllerTrackingAlgorithm*>(ctrTr)->getView());
	//QObject::connect(pv, &TrackerParameterView::trackingAreaType, this, &ControllerAreaDescriptor::trackingAreaType);
}

void ControllerAreaDescriptor::createModel()
{
	m_Model = new AreaInfo();
}
