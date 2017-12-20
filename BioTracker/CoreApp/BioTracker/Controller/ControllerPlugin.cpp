#include "ControllerPlugin.h"

#include "Controller/ControllerPlayer.h"
#include "Controller/ControllerTextureObject.h"
#include "Controller/ControllerTrackedComponentCore.h"
#include "ControllerMainWindow.h"
#include "QDebug"
#include "Model/PluginLoader.h"
#include "settings/Settings.h"
#include "util/types.h"
#include "Model/DataExporterCSV.h"
#include "ControllerDataExporter.h"
#include "ControllerAreaDescriptor.h"
#include "Controller/ControllerCoreParameter.h"


ControllerPlugin::ControllerPlugin(QObject* parent, IBioTrackerContext* context, ENUMS::CONTROLLERTYPE ctr) :
	IController(parent, context, ctr) {
	m_BioTrackerPlugin = NULL;

	m_TrackingThread = new QThread(this);
	m_TrackingThread->start();
}

ControllerPlugin::~ControllerPlugin() {
	m_TrackingThread->quit();
	m_TrackingThread->wait();

}

void ControllerPlugin::addToPluginList(QString str) {

	PluginLoader* loader = qobject_cast<PluginLoader*>(m_Model);
	loader->addToPluginList(str);

	// Add Plugin name to Main Window
	IController* ctrA = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
	QPointer< ControllerMainWindow > ctrMainWindow = qobject_cast<ControllerMainWindow*>(ctrA);

	ctrMainWindow->setTrackerList(qobject_cast<PluginLoader*>(m_Model)->getPluginMetaData(),
		qobject_cast<PluginLoader*>(m_Model)->getCurrentPluginName());
}

void ControllerPlugin::loadPluginFromFileName(QString str) {
    PluginLoader* loader = qobject_cast<PluginLoader*>(m_Model);

    if(loader->loadPluginFromFilename(str)) {

        createPlugin();

		// Add Plugin name to Main Window
		IController* ctrA = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
		QPointer< ControllerMainWindow > ctrMainWindow = qobject_cast<ControllerMainWindow*>(ctrA);

        ctrMainWindow->setTrackerList(qobject_cast<PluginLoader*>(m_Model)->getPluginMetaData(),
			qobject_cast<PluginLoader*>(m_Model)->getCurrentPluginName());

        //Add Tracker Parameter to Main Window
		IView *parms = m_BioTrackerPlugin->getTrackerParameterWidget();
        ctrMainWindow->setTrackerParamterWidget(parms);

		//Add Tracker tracked components (Elements) to Main Window

		//IView *v = m_BioTrackerPlugin->getTrackerElementsWidget();
		//ctrMainWindow->setTrackerElementsWidget(m_BioTrackerPlugin->getTrackerElementsWidget()); //MARKER
		IView *elems = m_BioTrackerPlugin->getTrackerElementsWidget();
		ctrMainWindow->setTrackerElementsWidget(elems); 



		IController* ctrB = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE);
		QPointer< ControllerTrackedComponentCore > ctrTrackedComponentCore = qobject_cast<ControllerTrackedComponentCore*>(ctrB);

		//Add Tracker tracked components model to ControllerTrackedComponentCore
		ctrTrackedComponentCore->addModel(m_BioTrackerPlugin->getTrackerComponentModel());
		//Add tracked component view to main window
		ctrMainWindow->setCoreElementsWidget(ctrTrackedComponentCore->getTrackingElementsWidgetCore());

		IController* ctrDE = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::DATAEXPORT);
		QPointer< ControllerDataExporter > ctrDataExp = qobject_cast<ControllerDataExporter*>(ctrDE);
		ctrDataExp->setDataStructure(m_BioTrackerPlugin->getTrackerComponentModel());
		ctrDataExp->setComponentFactory(m_BioTrackerPlugin->getComponentFactory());

		m_BioTrackerPlugin->sendCorePermissions();
		
	}
}

void ControllerPlugin::selectPlugin(QString str) {
	if (str.isEmpty())
		return; 
	PluginLoader* loader = qobject_cast<PluginLoader*>(m_Model);

	if (loader->getCurrentPluginName() != str)
        loadPluginFromFileName(loader->getPluginMap().find(str)->second);
}

void ControllerPlugin::createModel() {
	m_Model = new PluginLoader(this);
}

void ControllerPlugin::createView() {
}

void ControllerPlugin::connectModelToController() {
}

void ControllerPlugin::loadPluginsFromPluginSubfolder() {

	//////////////
	QDir d(CFG_DIR_PLUGINS);
	d.setFilter(QDir::Filter::Files);
	QStringList nameFilter;
	QFileInfoList fl = d.entryInfoList(nameFilter, QDir::Filter::Files);
	foreach(QFileInfo fi, fl) {
		QString s = fi.absoluteFilePath();
		std::string asdf = s.toStdString();
		addToPluginList(s);
	}

	BioTracker::Core::Settings *set = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
	std::string *usePlugins = (std::string*)(set->readValue("usePlugins"));
	if(usePlugins) {
		addToPluginList(usePlugins->c_str());
	}

} 

void ControllerPlugin::connectControllerToController() {

	// Add Plugin name to Main Window
	IController* ctrA = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
	QPointer< ControllerMainWindow > ctrMainWindow = qobject_cast<ControllerMainWindow*>(ctrA);

    ctrMainWindow->deactiveTrackingCheckBox();

	loadPluginsFromPluginSubfolder();

	// connect ControllerTrackedComponentView
	IController* ctrB = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE);
	QPointer< ControllerTrackedComponentCore > ctrTrackedComponentCore = qobject_cast<ControllerTrackedComponentCore*>(ctrB);

	QObject::connect(ctrTrackedComponentCore, SIGNAL(emitRemoveTrajectory(IModelTrackedTrajectory*)), this, 
		SLOT(receiveRemoveTrajectory(IModelTrackedTrajectory*)), Qt::DirectConnection);
	QObject::connect(ctrTrackedComponentCore, SIGNAL(emitRemoveTrackEntity(IModelTrackedTrajectory*)), this,
		SLOT(receiveRemoveTrackEntity(IModelTrackedTrajectory*)), Qt::DirectConnection);
	QObject::connect(ctrTrackedComponentCore, SIGNAL(emitAddTrajectory(QPoint)), this, 
		SLOT(receiveAddTrajectory(QPoint)), Qt::DirectConnection);
	QObject::connect(ctrTrackedComponentCore, SIGNAL(emitMoveElement(IModelTrackedTrajectory*, QPoint, int)), this, 
		SLOT(receiveMoveElement(IModelTrackedTrajectory*, QPoint, int)), Qt::DirectConnection);
	QObject::connect(ctrTrackedComponentCore, SIGNAL(emitSwapIds(IModelTrackedTrajectory*, IModelTrackedTrajectory*)), this,
		SLOT(receiveSwapIds(IModelTrackedTrajectory*, IModelTrackedTrajectory*)), Qt::DirectConnection);

	QObject::connect(this, SIGNAL(emitUpdateView()), ctrTrackedComponentCore,
		SLOT(receiveUpdateView()));

	// connect ControllerPlayer
	IController* ctrC = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
	QPointer< ControllerPlayer > ctrPlayer = qobject_cast<ControllerPlayer*>(ctrC);
	QObject::connect(ctrPlayer, SIGNAL(emitPauseState(bool)), this,
		SLOT(receivePauseState(bool)), Qt::DirectConnection);

	QObject::connect(ctrPlayer, SIGNAL(signalCurrentFrameNumberToPlugin(uint)),
		this, SLOT(receiveCurrentFrameNumberToPlugin(uint)), Qt::DirectConnection);

}

void ControllerPlugin::createPlugin() {
	m_BioTrackerPlugin = qobject_cast<PluginLoader*>(m_Model)->getPluginInstance();
	m_BioTrackerPlugin->createPlugin();

	m_BioTrackerPlugin->moveToThread(m_TrackingThread);

	connectPlugin();

	IController* ctrAreaDesc = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::AREADESCRIPTOR);
	ControllerAreaDescriptor* ctAreaDesc = qobject_cast<ControllerAreaDescriptor*>(ctrAreaDesc);
	ctAreaDesc->triggerUpdateAreaDescriptor();

	Q_EMIT signalCurrentFrameNumberToPlugin(m_currentFrameNumber);

}

void ControllerPlugin::connectPlugin() {
	IController* ctrA = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
	QPointer< ControllerPlayer > ctrPlayer = qobject_cast<ControllerPlayer*>(ctrA);

	IModel* model = ctrPlayer->getModel();

	IController* ctrB = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
	ControllerTextureObject* ctrTexture = qobject_cast<ControllerTextureObject*>(ctrB);
	
	IController* ctrAreaDesc = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::AREADESCRIPTOR);
	ControllerAreaDescriptor* ctAreaDesc = qobject_cast<ControllerAreaDescriptor*>(ctrAreaDesc);


	IController* ctrData = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::DATAEXPORT);
	ControllerDataExporter* ctDataEx = qobject_cast<ControllerDataExporter*>(ctrData);

	IController* ctrC = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE);
	ControllerTrackedComponentCore* ctrCompView = qobject_cast<ControllerTrackedComponentCore*>(ctrC);

	IController* ctrD = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COREPARAMETER);
	ControllerCoreParameter* ctrCoreParam = qobject_cast<ControllerCoreParameter*>(ctrD);

	QObject* obj = dynamic_cast<QObject*>(m_BioTrackerPlugin);

	QObject::connect(obj, SIGNAL(emitTrackingDone(uint)), ctDataEx, SLOT(receiveTrackingDone(uint)));

	QObject::connect(obj, SIGNAL(emitTrackingDone()), model, SLOT(receiveTrackingOperationDone()));
	QObject::connect(obj, SIGNAL(emitCvMat(std::shared_ptr<cv::Mat>, QString)),
					 ctrTexture, SLOT(receiveCvMat(std::shared_ptr<cv::Mat>, QString)));

	//TODO whyy do this two times??
	QObject::connect(obj, SIGNAL(emitTrackingDone(uint)), model, SLOT(receiveTrackingOperationDone()));

	QObject::connect(obj, SIGNAL(emitTrackingDone(uint)), ctrCompView, SLOT(receiveVisualizeTrackingModel(uint)));

	QObject::connect(obj, SIGNAL(emitChangeDisplayImage(QString)), ctrPlayer, SLOT(receiveChangeDisplayImage(QString)));

	QObject::connect(ctAreaDesc, SIGNAL(updateAreaDescriptor(IModelAreaDescriptor*)), obj, SLOT(receiveAreaDescriptor(IModelAreaDescriptor*)));

	QObject::connect(obj, SIGNAL(emitCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool>)), ctrCompView, 
		SLOT(setCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool>)));

	QObject::connect(obj, SIGNAL(emitCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool>)), ctrCoreParam,
		SLOT(setCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool>)));



	QObject::connect(this, SIGNAL(emitRemoveTrajectory(IModelTrackedTrajectory*)), obj, 
		SLOT(receiveRemoveTrajectory(IModelTrackedTrajectory*)), Qt::DirectConnection);
	QObject::connect(this, SIGNAL(emitRemoveTrackEntity(IModelTrackedTrajectory*)), obj,
		SLOT(receiveRemoveTrackEntity(IModelTrackedTrajectory*)), Qt::DirectConnection);
	QObject::connect(this, SIGNAL(emitAddTrajectory(QPoint)), obj, 
		SLOT(receiveAddTrajectory(QPoint)), Qt::DirectConnection);
	QObject::connect(this, SIGNAL(emitMoveElement(IModelTrackedTrajectory*, QPoint)), obj, 
		SLOT(receiveMoveElement(IModelTrackedTrajectory*, QPoint)), Qt::DirectConnection);
	QObject::connect(this, SIGNAL(emitSwapIds(IModelTrackedTrajectory*, IModelTrackedTrajectory*)), obj, 
		SLOT(receiveSwapIds(IModelTrackedTrajectory*, IModelTrackedTrajectory*)), Qt::DirectConnection);

	QObject::connect(this, SIGNAL(signalCurrentFrameNumberToPlugin(uint)), obj,
		SLOT(receiveCurrentFrameNumberFromMainApp(uint)), Qt::DirectConnection);
}

void ControllerPlugin::disconnectPlugin() {
	//    IController* ctrA = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
	//    QPointer< ControllerPlayer > ctrPlayer = qobject_cast<ControllerPlayer*>(ctrA);

	//    IModel* model = ctrPlayer->getModel();
	//    MediaPlayerStateMachine* player = dynamic_cast<MediaPlayerStateMachine*>(model);


	//    IController* ctrB = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
	//    QPointer< ControllerTextureObject > ctrTexture = qobject_cast<ControllerTextureObject*>(ctrB);


	//    //    QObject::disconnect(qobject_cast<QObject*> (m_BioTrackerPlugin), SIGNAL(emitCvMat(std::shared_ptr<cv::Mat>,QString)), ctrTexture, SLOT(receiveCvMat(std::shared_ptr<cv::Mat>,QString)));


	//    //    QObject::disconnect(player, SIGNAL(emitCurrentFrame(std::shared_ptr<cv::Mat>)), qobject_cast<QObject*> (m_BioTrackerPlugin), SLOT(receiveCvMat(std::shared_ptr<cv::Mat>)));

}

void ControllerPlugin::receiveRemoveTrajectory(IModelTrackedTrajectory * trajectory)
{
	if (m_paused) {
		emitRemoveTrajectory(trajectory);
		emitUpdateView();
	}
	else {
		//std::pair<EDIT, IModelTrackedTrajectory*> removeEdit(EDIT::REMOVE, trajectory);
		queueElement removeTrackEdit;
		removeTrackEdit.type = EDIT::REMOVE_TRACK;
		removeTrackEdit.trajectory0 = trajectory;
		m_editQueue.enqueue(removeTrackEdit);
	}
}

void ControllerPlugin::receiveRemoveTrackEntity(IModelTrackedTrajectory * trajectory)
{
	if (m_paused) {
		emitRemoveTrackEntity(trajectory);
		emitUpdateView();
	}
	else {
		//std::pair<EDIT, IModelTrackedTrajectory*> removeEdit(EDIT::REMOVE, trajectory);
		queueElement removeEntityEdit;
		removeEntityEdit.type = EDIT::REMOVE_ENTITY;
		removeEntityEdit.trajectory0 = trajectory;
		m_editQueue.enqueue(removeEntityEdit);
	}
}

void ControllerPlugin::receiveAddTrajectory(QPoint pos)
{
	if (m_paused) {
		emitAddTrajectory(pos);
		emitUpdateView();
	}
	else {
		queueElement addEdit;
		addEdit.type = EDIT::ADD;
		addEdit.pos = pos;
		m_editQueue.enqueue(addEdit);
	}
}

void ControllerPlugin::receiveMoveElement(IModelTrackedTrajectory * trajectory, QPoint pos, int toMove)
{
	if (m_paused) {
		emitMoveElement(trajectory, pos);
		//only emit the update after the last move is processed
		if (toMove == 1) {
			emitUpdateView();
		}
	}
	else {
		queueElement moveEdit;
		moveEdit.type = EDIT::MOVE;
		moveEdit.trajectory0 = trajectory;
		moveEdit.pos = pos;
		m_editQueue.enqueue(moveEdit);
	}
}

void ControllerPlugin::receiveSwapIds(IModelTrackedTrajectory * trajectory0, IModelTrackedTrajectory * trajectory1)
{
	if (m_paused) {
		emitSwapIds(trajectory0, trajectory1);
		emitUpdateView();
	}
	else {
		queueElement swapEdit;
		swapEdit.type = EDIT::SWAP;
		swapEdit.trajectory0 = trajectory0;
		swapEdit.trajectory1 = trajectory1;
		m_editQueue.enqueue(swapEdit);
	}
}

void ControllerPlugin::receivePauseState(bool state)
{
	m_paused = state;
}

void ControllerPlugin::receiveCurrentFrameNumberToPlugin(uint frameNumber)
{
	m_currentFrameNumber = frameNumber;
	Q_EMIT signalCurrentFrameNumberToPlugin(frameNumber);
}

void ControllerPlugin::sendCurrentFrameToPlugin(std::shared_ptr<cv::Mat> mat, uint number) {
	m_currentFrameNumber = number;

	//Prevent calling the plugin if none is loaded
	if (m_BioTrackerPlugin) {
		while (!m_editQueue.isEmpty()) {
			queueElement edit = m_editQueue.dequeue();

			switch (edit.type)
			{
			case EDIT::REMOVE_TRACK:
				emitRemoveTrajectory(edit.trajectory0);
				break;
			case EDIT::REMOVE_ENTITY:
				emitRemoveTrackEntity(edit.trajectory0);
				break;
			case EDIT::ADD:
				emitAddTrajectory(edit.pos);
				break;
			case EDIT::MOVE:
				emitMoveElement(edit.trajectory0, edit.pos);
				break;
			case EDIT::SWAP:
				emitSwapIds(edit.trajectory0, edit.trajectory1);
				break;
			}
		}
		m_BioTrackerPlugin->receiveCurrentFrameFromMainApp(mat, number);
	}
}

void ControllerPlugin::receiveTrackingDone() {

}
