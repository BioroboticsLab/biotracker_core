#include "ControllerAreaDescriptor.h"

#include "Model/AreaDescriptor/AreaInfo.h"
#include "View/AreaDesciptor/AreaDescriptor.h"
#include "View/AreaDesciptor/RectDescriptor.h"
#include "View/AreaDesciptor/EllipseDescriptor.h"
#include "settings/Settings.h"
#include "util/types.h"

#include "ControllerGraphicScene.h"
#include "View/GraphicsView.h"
#include "Model/AreaDescriptor/Rectification.h"

#include <iostream>

ControllerAreaDescriptor::ControllerAreaDescriptor(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
	IController(parent, context, ctr)
{
	_watchingVertice = -1;
	_watchingVerticeType = -1; 
	m_ViewApperture = nullptr;
}

void ControllerAreaDescriptor::triggerUpdateAreaDescriptor() {

	IModelAreaDescriptor* area = dynamic_cast<IModelAreaDescriptor*>(getModel());
	Q_EMIT updateAreaDescriptor(area);
}

void ControllerAreaDescriptor::createView()
{
	//IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COMPONENT);

	assert(m_Model);
	AreaInfo* mInf = dynamic_cast<AreaInfo*> (getModel());
	RectDescriptor* view = new RectDescriptor(this, mInf->_rect.get());
	m_View = view;
	

	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);
	auto viewController = qobject_cast<ControllerGraphicScene*>(ctr);
	auto gview = dynamic_cast<GraphicsView*> (viewController->getView());
	gview->addGraphicsItem(view);

	AreaInfo* area = dynamic_cast<AreaInfo*>(getModel());
	
	//m_ViewApperture = new RectDescriptor(view, this, area->_rect.get());
	//QObject::connect(view, &RectDescriptor::updatedPoints, view, &AreaDescriptorView::updateRectification);

	BioTracker::Core::Settings *_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
	int v = _settings->getValueOrDefault<int>(AREADESCRIPTOR::CN_APPERTURE_TYPE, 0);
	trackingAreaType(v);
}

void ControllerAreaDescriptor::connectModelToController()
{

}

void ControllerAreaDescriptor::trackingAreaType(int v) {

	IController* ctr1 = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);
	auto viewController = qobject_cast<ControllerGraphicScene*>(ctr1);
	auto gview = dynamic_cast<GraphicsView*> (viewController->getView());

	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COMPONENT);
	RectDescriptor* view = dynamic_cast<RectDescriptor*>(m_View);
	AreaInfo* area = dynamic_cast<AreaInfo*>(getModel());
	if (m_ViewApperture) {
		gview->removeGraphicsItem(static_cast<AreaDescriptor*>(m_ViewApperture));
		delete m_ViewApperture;
	}

	BioTracker::Core::Settings *_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
	if (v == 0) {
		m_ViewApperture = new RectDescriptor(this, area->_apperture.get());
		area->_apperture->_type = 0;
		static_cast<RectDescriptor*>(m_ViewApperture)->setBrush(QBrush(Qt::red));
		//QObject::connect(m_ViewApperture, &RectDescriptor::updateRect, view, &AreaDescriptorView::updateApperture);
		_settings->setParam(AREADESCRIPTOR::CN_APPERTURE_TYPE, 0);
	}
	else if (v > 0) {
		m_ViewApperture = new EllipseDescriptor(this, area->_apperture.get());
		area->_apperture->_type = 1;
		static_cast<EllipseDescriptor*>(m_ViewApperture)->setBrush(QBrush(Qt::red));
		//QObject::connect(view->_tracking, &EllipseDescriptor::updatedPoints, view, &AreaDescriptorView::updateApperture);
		_settings->setParam(AREADESCRIPTOR::CN_APPERTURE_TYPE, 1);
	}

	gview->addGraphicsItem(static_cast<AreaDescriptor*>(m_ViewApperture));
}

void ControllerAreaDescriptor::connectControllerToController()
{
	{
		IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);
		auto viewController = qobject_cast<ControllerGraphicScene*>(ctr);
		auto view = dynamic_cast<GraphicsView*> (viewController->getView());
		AreaDescriptor* rd = static_cast<AreaDescriptor*>(getView());
		AreaDescriptor* ad = static_cast<AreaDescriptor*>(m_ViewApperture);
		view->addGraphicsItem(rd);
		view->addGraphicsItem(ad);

		QObject::connect(view, &GraphicsView::onMousePressEvent, this, &ControllerAreaDescriptor::mousePressEvent, Qt::DirectConnection);
		QObject::connect(view, &GraphicsView::onMouseReleaseEvent, this, &ControllerAreaDescriptor::mouseReleaseEvent, Qt::DirectConnection);
		QObject::connect(view, &GraphicsView::onMouseMoveEvent, this, &ControllerAreaDescriptor::mouseMoveEvent, Qt::DirectConnection);
		QObject::connect(view, &GraphicsView::onKeyReleaseEvent, this, &ControllerAreaDescriptor::keyReleaseEvent, Qt::DirectConnection);

		QWidget *viewport = view->viewport();
		QObject::connect(this, SIGNAL(onRepaintRequired()), viewport, SLOT(repaint()));

		AreaInfo* area = dynamic_cast<AreaInfo*>(getModel());
		QObject::connect(area->_rect.get(), SIGNAL(updatedVertices()), this, SLOT(updateView()));
		QObject::connect(area->_apperture.get(), SIGNAL(updatedVertices()), this, SLOT(updateView()));
	}
	{
		//View stuff from the parameter view
		IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);
		auto viewController = qobject_cast<ControllerGraphicScene*>(ctr);
	}
}

void ControllerAreaDescriptor::createModel()
{
	m_Model = new AreaInfo(0);
}

void ControllerAreaDescriptor::updateView() {

	AreaInfo* area = static_cast<AreaInfo*>(getModel());
	RectDescriptor* rd = static_cast<RectDescriptor*>(getView());
	rd->updateRect();
	AreaDescriptor* ad = static_cast<AreaDescriptor*>(m_ViewApperture);
	ad->updateRect();
	area->updateRectification();
	area->updateApperture();
}


void ControllerAreaDescriptor::keyReleaseEvent(QKeyEvent *event)
{
}

void ControllerAreaDescriptor::mousePressEvent(QMouseEvent *event, const QPoint &pos)
{
	auto model = static_cast<AreaInfo*>(getModel());

	int verticeRect = model->_rect->getVerticeAtLocation(pos);
	if (verticeRect >= 0) {
		_watchingVertice = verticeRect;
		_watchingVerticeType = 0;
		event->accept();
	}

	int verticeApp = model->_apperture->getVerticeAtLocation(pos);
	if (verticeApp >= 0) {
		_watchingVertice = verticeApp;
		_watchingVerticeType = 1;
		event->accept();
	}

	if (verticeRect < 0 && verticeApp < 0) {
		_watchingVertice = -1;
		_watchingVerticeType = -1;
	}
}

void ControllerAreaDescriptor::mouseReleaseEvent(QMouseEvent*event, const QPoint &pos)
{
	auto model = static_cast<AreaInfo*>(getModel());

	if (_watchingVertice >= 0 && _watchingVerticeType == 0) {
		model->_rect->setVerticeAtLocation(pos, _watchingVertice);
		event->accept();
		triggerUpdateAreaDescriptor();
	}

	if (_watchingVertice >= 0 && _watchingVerticeType == 1) {
		model->_apperture->setVerticeAtLocation(pos, _watchingVertice);
		event->accept();
		triggerUpdateAreaDescriptor();
	}
}

void ControllerAreaDescriptor::mouseMoveEvent(QMouseEvent*event, const QPoint &pos)
{

}

void ControllerAreaDescriptor::setRectificationDimensions(double w, double h) {

	AreaInfo* area = static_cast<AreaInfo*>(getModel());
	area->setRectificationDimensions(w, h);
	triggerUpdateAreaDescriptor();
	BioTracker::Core::Settings *_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
	_settings->setParam(AREADESCRIPTOR::RECT_W, w);
	_settings->setParam(AREADESCRIPTOR::RECT_H, h);
}

void ControllerAreaDescriptor::setDisplayRectificationDefinition(bool b) {
	RectDescriptor* rd = static_cast<RectDescriptor*>(getView());
	rd->setVisible(b);
}

void ControllerAreaDescriptor::setDisplayTrackingAreaDefinition(bool b) {
	AreaDescriptor* ad = static_cast<AreaDescriptor*>(m_ViewApperture);
	ad->setVisible(b);
}

void ControllerAreaDescriptor::setTrackingAreaAsEllipse(bool b) {
	//Not passing b as a parameter for clarification reasons
	if (b) {
		trackingAreaType(1);
	}
	else {
		trackingAreaType(0);
	}
}


