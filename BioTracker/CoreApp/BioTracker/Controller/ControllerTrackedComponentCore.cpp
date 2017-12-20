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
#include "Interfaces/IModel/IModelTrackedTrajectory.h"

ControllerTrackedComponentCore::ControllerTrackedComponentCore(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
	IController(parent, context, ctr)
{
	m_View = nullptr;
	m_Model = nullptr;
}

void ControllerTrackedComponentCore::createView()
{
	//This occurs when there has been a previous plugin to visualize
    if (m_View != nullptr)
        delete m_View;

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

	//connect to update track number in core params
	IController * ctrICP = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COREPARAMETER);
	QPointer< ControllerCoreParameter > ctrCP = qobject_cast<ControllerCoreParameter *>(ctrICP);
	QObject::connect(this, SIGNAL(emitTrackNumber(int)), ctrCP, SLOT(receiveTrackNumber(int)));

    // Tell the Visualization to reset upon loading a new plugin
    QObject::connect(ctrMainWindow, &ControllerMainWindow::emitPluginLoaded, this, &ControllerTrackedComponentCore::receiveOnPluginLoaded);
}

void ControllerTrackedComponentCore::receiveOnPluginLoaded() {
   //createView();
   //connectControllerToController();
   //TrackedComponentView* compView = dynamic_cast<TrackedComponentView*>(m_View);
   //compView->getNotified();
   //dynamic_cast<TrackedComponentView*>(m_View)->createChildShapesAtStart();
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

void ControllerTrackedComponentCore::receiveRemoveTrackEntity(IModelTrackedTrajectory* trajectory)
{
	emitRemoveTrackEntity(trajectory);
}

void ControllerTrackedComponentCore::receiveAddTrajectory(QPoint pos)
{
	emitAddTrajectory(pos);
}

void ControllerTrackedComponentCore::receiveMoveElement(IModelTrackedTrajectory * trajectory, QPoint pos, int toMove)
{
	emitMoveElement(trajectory, pos, toMove);
}

void ControllerTrackedComponentCore::receiveSwapIds(IModelTrackedTrajectory * trajectory0, IModelTrackedTrajectory * trajectory1)
{
	emitSwapIds(trajectory0, trajectory1);
}

void ControllerTrackedComponentCore::receiveUpdateView()
{
	TrackedComponentView* compView = dynamic_cast<TrackedComponentView*>(m_View);
	compView->getNotified();
	//signal the core parameter controller to update the track number
	IModelTrackedTrajectory *model = dynamic_cast<IModelTrackedTrajectory *>(getModel());
	int trackNumber = model->validCount();
	emitTrackNumber(trackNumber);
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
	
	//signal initial track number to core params
	IModelTrackedTrajectory *iModel = dynamic_cast<IModelTrackedTrajectory *>(getModel());
	int trackNumber = iModel->validCount();
	emitTrackNumber(trackNumber);
}

void ControllerTrackedComponentCore::receiveVisualizeTrackingModel(uint framenumber)
{
	//signal the view to update track entities
	TrackedComponentView* compView = dynamic_cast<TrackedComponentView*>(m_View);
	compView->updateShapes(framenumber);
	//signal the core parameter controller to update the track number
	IModelTrackedTrajectory *model = dynamic_cast<IModelTrackedTrajectory *>(getModel());
	if (model) {
		int trackNumber = model->validCount();
		emitTrackNumber(trackNumber);
	}
}
