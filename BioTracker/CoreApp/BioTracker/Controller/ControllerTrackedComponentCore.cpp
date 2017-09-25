#include "ControllerTrackedComponentCore.h"
#include "Model/null_Model.h"
#include "View/TrackedComponentView.h"
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

void ControllerTrackedComponentCore::addModel(IModel* model)
{
	m_Model = model;
	m_View = new TrackedComponentView(0, this, m_Model);

	QObject::connect(dynamic_cast<TrackedComponentView*>(m_View), SIGNAL(emitAddTrajectory(QPoint)), this, SLOT(receiveAddTrajectory(QPoint)));
	QObject::connect(dynamic_cast<TrackedComponentView*>(m_View), SIGNAL(emitSwapIds(IModelTrackedTrajectory*, IModelTrackedTrajectory*)), this, SLOT(receiveSwapIds(IModelTrackedTrajectory*, IModelTrackedTrajectory*)));

}

void ControllerTrackedComponentCore::receiveTrackingOperationDone(uint framenumber) 
{
	TrackedComponentView* compView = dynamic_cast<TrackedComponentView*>(m_View);
	compView->updateShapes(framenumber);
}