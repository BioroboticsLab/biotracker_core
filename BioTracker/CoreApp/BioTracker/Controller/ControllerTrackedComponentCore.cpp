#include "ControllerTrackedComponentCore.h"
#include "ControllerMainWindow.h"
#include "ControllerCoreParameter.h"
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
	m_View = nullptr;
	m_Model = nullptr;
}

void ControllerTrackedComponentCore::createView()
{
	//m_View = new TrackedComponentView(0, this, m_Model);
	m_View = new TrackedComponentView(0, this, m_Model);
}

void ControllerTrackedComponentCore::connectModelToController()
{

}

void ControllerTrackedComponentCore::connectControllerToController()
{
	IController * ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
	QPointer< ControllerMainWindow > ctrMainWindow = qobject_cast<ControllerMainWindow *>(ctr);

	TrackedComponentView* view = static_cast<TrackedComponentView*>(m_View);
	QObject::connect(view, SIGNAL(emitAddTrajectory(QPoint)), this, SLOT(receiveAddTrajectory(QPoint)));
	QObject::connect(view, SIGNAL(emitSwapIds(IModelTrackedTrajectory*, IModelTrackedTrajectory*)), this, SLOT(receiveSwapIds(IModelTrackedTrajectory*, IModelTrackedTrajectory*)));
}


void ControllerTrackedComponentCore::setCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool> permission)
{
	if (dynamic_cast<TrackedComponentView*>(m_View)) {
		dynamic_cast<TrackedComponentView*>(m_View)->setPermission(permission);
	}
	else {
		qDebug() << "no view yet";
		//This should never happen, actually
		assert(false);
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

IModel* ControllerTrackedComponentCore::getCoreParameter()
{
	IController * ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COREPARAMETER);
	QPointer< ControllerCoreParameter > ctrCP = qobject_cast<ControllerCoreParameter *>(ctr);
	return ctrCP->getModel();
}

void ControllerTrackedComponentCore::addModel(IModel* model)
{
	m_Model = model;
	m_View->setNewModel(m_Model);
}

void ControllerTrackedComponentCore::receiveTrackingOperationDone(uint framenumber) 
{
	TrackedComponentView* compView = dynamic_cast<TrackedComponentView*>(m_View);
	compView->updateShapes(framenumber);
}