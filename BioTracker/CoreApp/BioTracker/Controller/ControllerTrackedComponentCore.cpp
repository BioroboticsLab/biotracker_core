#include "ControllerTrackedComponentCore.h"
#include "ControllerMainWindow.h"
#include "Model/null_Model.h"
#include "Model/CoreParameter.h"
#include "View/TrackedComponentView.h"
#include "View/CoreParameterView.h"
#include "settings/Settings.h"
#include "util/types.h"
#include "qdebug.h"
#include "Interfaces\IModel\IModelTrackedTrajectory.h"

ControllerTrackedComponentCore::ControllerTrackedComponentCore(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
	IController(parent, context, ctr)
{

}

void ControllerTrackedComponentCore::createView()
{
	//m_View = new TrackedComponentView(0, this, m_Model);
}

void ControllerTrackedComponentCore::connectModelToController()
{

}

void ControllerTrackedComponentCore::connectControllerToController()
{
	IController * ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
	QPointer< ControllerMainWindow > ctrMainWindow = qobject_cast<ControllerMainWindow *>(ctr);

	QObject::connect(this, &ControllerTrackedComponentCore::emitCoreParameterView, ctrMainWindow, &ControllerMainWindow::receiveCoreParameterView, Qt::DirectConnection);
}


void ControllerTrackedComponentCore::setCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool> permission)
{
	if (dynamic_cast<TrackedComponentView*>(m_View)) {
		dynamic_cast<TrackedComponentView*>(m_View)->setPermission(permission);
	}
	else {
		qDebug() << "no view yet";
	}
}

void ControllerTrackedComponentCore::receiveRemoveTrajectory(IModelTrackedTrajectory* trajectory)
{
	emitRemoveTrajectory(trajectory);
}

void ControllerTrackedComponentCore::receiveAddTrajectory(QPoint pos)
{
	emitAddTrajectory(pos);
}

void ControllerTrackedComponentCore::receiveMoveElement(IModelTrackedTrajectory * trajectory, QPoint pos)
{
	emitMoveElement(trajectory, pos);
}

void ControllerTrackedComponentCore::receiveSwapIds(IModelTrackedTrajectory * trajectory0, IModelTrackedTrajectory * trajectory1)
{
	emitSwapIds(trajectory0, trajectory1);
}

void ControllerTrackedComponentCore::createModel()
{
	// This controller gets his model (via addModel()) from the corresponding tracked-component-controller when a plugin is loaded
	m_Model = new null_Model();
}


IView *ControllerTrackedComponentCore::getTrackingElementsWidgetCore()
{
	return m_View;
}

//TODO: view creation in createView()...
void ControllerTrackedComponentCore::addModel(IModel* model)
{
	m_Model = model;
	m_View = new TrackedComponentView(0, this, m_Model);

	QObject::connect(dynamic_cast<TrackedComponentView*>(m_View), SIGNAL(emitAddTrajectory(QPoint)), this, SLOT(receiveAddTrajectory(QPoint)));
	QObject::connect(dynamic_cast<TrackedComponentView*>(m_View), SIGNAL(emitSwapIds(IModelTrackedTrajectory*, IModelTrackedTrajectory*)), this, SLOT(receiveSwapIds(IModelTrackedTrajectory*, IModelTrackedTrajectory*)));

	//TODO set model for parameters
	m_coreParameterModel = new CoreParameter(this);
	m_parameterView = new CoreParameterView(0, this, m_coreParameterModel);

	QObject::connect(dynamic_cast<CoreParameterView*>(m_parameterView), &CoreParameterView::emitViewSwitch, dynamic_cast<TrackedComponentView*>(m_View), &TrackedComponentView::receiveViewSwitch, Qt::DirectConnection);
	QObject::connect(dynamic_cast<CoreParameterView*>(m_parameterView), &CoreParameterView::emitSelectAll, dynamic_cast<TrackedComponentView*>(m_View), &TrackedComponentView::receiveSelectAll, Qt::DirectConnection);
	QObject::connect(dynamic_cast<CoreParameterView*>(m_parameterView), &CoreParameterView::emitColorChangeBorderAll, dynamic_cast<TrackedComponentView*>(m_View), &TrackedComponentView::receiveColorChangeBorderAll, Qt::DirectConnection);
	QObject::connect(dynamic_cast<CoreParameterView*>(m_parameterView), &CoreParameterView::emitColorChangeBrushAll, dynamic_cast<TrackedComponentView*>(m_View), &TrackedComponentView::receiveColorChangeBrushAll, Qt::DirectConnection);
	QObject::connect(dynamic_cast<CoreParameterView*>(m_parameterView), &CoreParameterView::emitTracingHistoryLength, dynamic_cast<TrackedComponentView*>(m_View), &TrackedComponentView::receiveTracingHistoryLength, Qt::DirectConnection);
	QObject::connect(dynamic_cast<CoreParameterView*>(m_parameterView), &CoreParameterView::emitTracingStyle, dynamic_cast<TrackedComponentView*>(m_View), &TrackedComponentView::receiveTracingStyle, Qt::DirectConnection);
	QObject::connect(dynamic_cast<CoreParameterView*>(m_parameterView), &CoreParameterView::emitTracingSteps, dynamic_cast<TrackedComponentView*>(m_View), &TrackedComponentView::receiveTracingSteps, Qt::DirectConnection);

	emitCoreParameterView(m_parameterView);
}

IModel * ControllerTrackedComponentCore::getCoreParameter()
{
	return m_coreParameterModel;
}

void ControllerTrackedComponentCore::receiveTrackingOperationDone(uint framenumber) 
{
	TrackedComponentView* compView = dynamic_cast<TrackedComponentView*>(m_View);
	compView->updateShapes(framenumber);
}