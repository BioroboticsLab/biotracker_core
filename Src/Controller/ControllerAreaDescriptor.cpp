#include "ControllerAreaDescriptor.h"

#include "Model/AreaDescriptor/AreaInfo.h"
#include "View/AreaDesciptor/AreaDescriptor.h"
#include "View/AreaDesciptor/RectDescriptor.h"
#include "View/AreaDesciptor/EllipseDescriptor.h"
#include "util/types.h"

#include "ControllerGraphicScene.h"
#include "View/GraphicsView.h"
#include "Model/AreaDescriptor/Rectification.h"
#include "Controller/ControllerPlayer.h"
#include "Controller/ControllerCoreParameter.h"

#include <iostream>

#include "Model/AreaDescriptor/AreaMemory.h"
using namespace AreaMemory;

ControllerAreaDescriptor::ControllerAreaDescriptor(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
	IControllerCfg(parent, context, ctr)
{
	_watchingVertice = -1;
	_watchingVerticeType = BiotrackerTypes::AreaType::NONE;
	m_ViewApperture = nullptr;
}

void ControllerAreaDescriptor::triggerUpdateAreaDescriptor() {

	IModelAreaDescriptor* area = dynamic_cast<IModelAreaDescriptor*>(getModel());
	Q_EMIT updateAreaDescriptor(area);
}

void ControllerAreaDescriptor::createView()
{
	assert(m_Model);
	AreaInfo* mInf = dynamic_cast<AreaInfo*> (getModel());
	RectDescriptor* view = new RectDescriptor(this, mInf->_rect.get());
	m_View = view;
    QObject::connect(this, &ControllerAreaDescriptor::currentVectorDrag, view, &RectDescriptor::receiveDragUpdate);

	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);
	auto viewController = qobject_cast<ControllerGraphicScene*>(ctr);
	auto gview = dynamic_cast<GraphicsView*> (viewController->getView());
	gview->addGraphicsItem(view);

	AreaInfo* area = dynamic_cast<AreaInfo*>(getModel());

	int v = _cfg->AppertureType;
	trackingAreaType(v);

	// set the rectification dimensions
	double h = std::max(_cfg->RectificationHeight, std::numeric_limits<double>::epsilon());
    double w = std::max(_cfg->RectificationWidth, std::numeric_limits<double>::epsilon());
	setRectificationDimensions(w, h);
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

	if (v == 0) {
		m_ViewApperture = new RectDescriptor(this, area->_apperture.get());
		area->_apperture->setType(0);
		static_cast<RectDescriptor*>(m_ViewApperture)->setBrush(QBrush(Qt::red));
        QObject::connect(this, &ControllerAreaDescriptor::currentVectorDrag, static_cast<RectDescriptor*>(m_ViewApperture), &RectDescriptor::receiveDragUpdate);
		_cfg->AppertureType = 0;
	}
	else if (v > 0) {
		m_ViewApperture = new EllipseDescriptor(this, area->_apperture.get());
		area->_apperture->setType(1);
		static_cast<EllipseDescriptor*>(m_ViewApperture)->setBrush(QBrush(Qt::red));
        static_cast<EllipseDescriptor*>(m_ViewApperture)->setDimensions(_w, _h);
        QObject::connect(this, &ControllerAreaDescriptor::currentVectorDrag, static_cast<EllipseDescriptor*>(m_ViewApperture), &EllipseDescriptor::receiveDragUpdate);
		_cfg->AppertureType = 1;
	}

    if (!_visibleApperture)
        static_cast<AreaDescriptor*>(m_ViewApperture)->hide();
    if (!_visibleRectification)
        static_cast<AreaDescriptor*>(m_View)->hide();

	gview->addGraphicsItem(static_cast<AreaDescriptor*>(m_ViewApperture));
}

void ControllerAreaDescriptor::rcvPlayerParameters(playerParameters* parameters)
{
    //Best effort to save performance...
    //File has changed
    if (_currentFilename != parameters->m_CurrentFilename)
    {
        _currentFilename = parameters->m_CurrentFilename;

        //Set area descriptor dimensions
        AreaDescriptor* ad = static_cast<AreaDescriptor*>(m_ViewApperture);
        std::shared_ptr<cv::Mat> m = parameters->m_CurrentFrame;
        if (ad && m != nullptr) {
            _w = m->size().width;
            _h = m->size().height;
            ad->setDimensions(_w, _h);
        }
        QVector<QString> v = getVertices(_currentFilename, _cfg->AreaDefinitions);
        if (!v.empty()) {
            changeAreaDescriptorType(v[2]);
        }
    }
    
}

void ControllerAreaDescriptor::connectControllerToController()
{
	{
		IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);
		auto viewController = qobject_cast<ControllerGraphicScene*>(ctr);
		auto view = dynamic_cast<GraphicsView*> (viewController->getView());

		QObject::connect(view, &GraphicsView::onMousePressEvent, this, &ControllerAreaDescriptor::mousePressEvent, Qt::DirectConnection);
		QObject::connect(view, &GraphicsView::onMouseReleaseEvent, this, &ControllerAreaDescriptor::mouseReleaseEvent, Qt::DirectConnection);
		QObject::connect(view, &GraphicsView::onMouseMoveEvent, this, &ControllerAreaDescriptor::mouseMoveEvent, Qt::DirectConnection);
        QObject::connect(view, &GraphicsView::onKeyReleaseEvent, this, &ControllerAreaDescriptor::keyReleaseEvent, Qt::DirectConnection);

        //Area info model needs to know video dimensions
        auto model = static_cast<AreaInfo*>(getModel()); 
        IController* ctrPl = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
        auto mediaPlayerController = static_cast<ControllerPlayer*> (ctrPl);
        MediaPlayer *player = static_cast<MediaPlayer*> (mediaPlayerController->getModel());
        QObject::connect(player, &MediaPlayer::fwdPlayerParameters, model, &AreaInfo::rcvPlayerParameters, Qt::DirectConnection);
        QObject::connect(player, &MediaPlayer::fwdPlayerParameters, this, &ControllerAreaDescriptor::rcvPlayerParameters, Qt::DirectConnection);


        IController* ctrParms = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COREPARAMETER);
        auto parmsController = qobject_cast<ControllerCoreParameter*>(ctrParms);
        QObject::connect(this, &ControllerAreaDescriptor::changeAreaDescriptorType, parmsController, &ControllerCoreParameter::changeAreaDescriptorType, Qt::DirectConnection);


		AreaInfo* area = dynamic_cast<AreaInfo*>(getModel());
		QObject::connect(area->_rect.get(), SIGNAL(updatedVertices()), this, SLOT(updateView()));
		QObject::connect(area->_apperture.get(), SIGNAL(updatedVertices()), this, SLOT(updateView()));
	}
	{
		//View stuff from the parameter view
		IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);
		auto viewController = qobject_cast<ControllerGraphicScene*>(ctr);
	}
    {
        IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COREPARAMETER);
        ControllerCoreParameter *cctr = static_cast<ControllerCoreParameter*>(ctr);
        cctr->triggerUpdate();
    }
}

void ControllerAreaDescriptor::createModel()
{
	m_Model = new AreaInfo(this);
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

	//check if rectification vertice is grabbed and visible
	RectDescriptor* rd = static_cast<RectDescriptor*>(getView());
	int verticeRect = -1;
	if (rd->isVisible()) {
		verticeRect = model->_rect->getVerticeAtLocation(pos);
		if (verticeRect >= 0) {
			_watchingVertice = verticeRect;
			_watchingVerticeType = BiotrackerTypes::AreaType::RECT;
			event->accept();
		}
	}


	//check if apperture vertice is grabbed and visible
	AreaDescriptor* ad = static_cast<AreaDescriptor*>(m_ViewApperture);
	int verticeApp = -1;
	if (ad->isVisible()) {
		verticeApp = model->_apperture->getVerticeAtLocation(pos);
		if (verticeApp >= 0) {
			_watchingVertice = verticeApp;
			_watchingVerticeType = BiotrackerTypes::AreaType::APPERTURE;
			event->accept();
		}
	}

	//else none is grabbed
	if (verticeRect < 0 && verticeApp < 0) {
		_watchingVertice = -1;
		_watchingVerticeType = BiotrackerTypes::AreaType::NONE;
	}

	//disable use entire screen when one vertice is grabbed
    if (verticeRect > 0 || verticeApp > 0) {
        AreaInfo* area = static_cast<AreaInfo*>(getModel());
        area->setUseEntireScreen(false);
    }

    Q_EMIT currentVectorDrag(_watchingVerticeType, _watchingVertice, pos.x(), pos.y());
}

void ControllerAreaDescriptor::mouseReleaseEvent(QMouseEvent*event, const QPoint &pos)
{
	auto model = static_cast<AreaInfo*>(getModel());

	if (_watchingVertice >= 0 && _watchingVerticeType == BiotrackerTypes::AreaType::RECT) {
		model->_rect->setVerticeAtLocation(pos, _watchingVertice);
		event->accept();
		triggerUpdateAreaDescriptor();
	}

	if (_watchingVertice >= 0 && _watchingVerticeType == BiotrackerTypes::AreaType::APPERTURE) {
		model->_apperture->setVerticeAtLocation(pos, _watchingVertice);
		event->accept();
		triggerUpdateAreaDescriptor();
	}

    _watchingVerticeType = BiotrackerTypes::AreaType::NONE;
    _watchingVertice = -1;
    Q_EMIT currentVectorDrag(_watchingVerticeType, _watchingVertice, pos.x(), pos.y());
}

void ControllerAreaDescriptor::mouseMoveEvent(QMouseEvent*event, const QPoint &pos)
{
    Q_EMIT currentVectorDrag(_watchingVerticeType, _watchingVertice, pos.x(), pos.y());
}

void ControllerAreaDescriptor::setRectificationDimensions(double w, double h) {

	_cfg->RectificationHeight = h;
	_cfg->RectificationWidth = w;

	AreaInfo* area = static_cast<AreaInfo*>(getModel());
	area->setRectificationDimensions(w, h);
	triggerUpdateAreaDescriptor();

    RectDescriptor* rd = static_cast<RectDescriptor*>(getView());
	if(rd){
    	rd->updateRect();
	}
}

void ControllerAreaDescriptor::setDisplayRectificationDefinition(bool b) {
    _visibleRectification = b;
	RectDescriptor* rd = static_cast<RectDescriptor*>(getView());
	rd->setVisible(b);
}

void ControllerAreaDescriptor::setDisplayTrackingAreaDefinition(bool b) {
    _visibleApperture = b;
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


