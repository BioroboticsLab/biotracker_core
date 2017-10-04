#include "ControllerCoreParameter.h"
#include "ControllerTrackedComponentCore.h"
#include "ControllerAreaDescriptor.h"
#include "View/CoreParameterView.h"
#include "View/TrackedComponentView.h"
#include "Model/CoreParameter.h"

ControllerCoreParameter::ControllerCoreParameter(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
	IController(parent, context, ctr)
{

}

void ControllerCoreParameter::createView()
{
	assert(m_Model);
	m_View = new CoreParameterView(0, this, m_Model);
}

void ControllerCoreParameter::connectModelToController()
{

}

void ControllerCoreParameter::connectControllerToController()
{
	CoreParameterView* view = static_cast<CoreParameterView*>(m_View);
	//Connections to the trackedComponentCore
	{
		IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE);
		ControllerTrackedComponentCore *tccController = dynamic_cast<ControllerTrackedComponentCore*>(ctr);
		IView* vv = tccController->getView();
		TrackedComponentView* tcview = static_cast<TrackedComponentView*>(vv);

		QObject::connect(view, &CoreParameterView::emitViewSwitch, tcview, &TrackedComponentView::receiveViewSwitch, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitSelectAll, tcview, &TrackedComponentView::receiveSelectAll, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitColorChangeBorderAll, tcview, &TrackedComponentView::receiveColorChangeBorderAll, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitColorChangeBrushAll, tcview, &TrackedComponentView::receiveColorChangeBrushAll, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitTracingHistoryLength, tcview, &TrackedComponentView::receiveTracingHistoryLength, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitTracingStyle, tcview, &TrackedComponentView::receiveTracingStyle, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitTracingSteps, tcview, &TrackedComponentView::receiveTracingSteps, Qt::DirectConnection);
	}
	//Connections to the AreaDescriptor
	{
		IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::AREADESCRIPTOR);
		ControllerAreaDescriptor *adController = static_cast<ControllerAreaDescriptor*>(ctr);
		QObject::connect(view, &CoreParameterView::emitRectDimensions, adController, &ControllerAreaDescriptor::setRectificationDimensions, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitDisplayTrackingArea, adController, &ControllerAreaDescriptor::setDisplayTrackingAreaDefinition, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitDisplayRectification, adController, &ControllerAreaDescriptor::setDisplayRectificationDefinition, Qt::DirectConnection);
		QObject::connect(view, &CoreParameterView::emitTrackingAreaAsEllipse, adController, &ControllerAreaDescriptor::setTrackingAreaAsEllipse, Qt::DirectConnection);
	}

}

void ControllerCoreParameter::createModel()
{

	m_Model = new CoreParameter(this);
}

void ControllerCoreParameter::updateView() {

//	RectDescriptor* rd = static_cast<RectDescriptor*>(getView());
//	rd->updateRect();
}
