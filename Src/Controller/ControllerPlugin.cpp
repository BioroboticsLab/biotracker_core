#include "ControllerPlugin.h"

#include "Controller/ControllerPlayer.h"
#include "Controller/ControllerTextureObject.h"
#include "Controller/ControllerTrackedComponentCore.h"
#include "ControllerMainWindow.h"
#include "QDebug"
#include "util/types.h"
#include "ControllerDataExporter.h"
#include "ControllerAreaDescriptor.h"
#include "Controller/ControllerCoreParameter.h"
#include "Controller/ControllerCommands.h"

#define REGISTRY_PATH "SOFTWARE\\FUBioroboticsLab\\BioTracker\\Plugins"
#define TRACKER_SUFFIX ".bio_tracker"

ControllerPlugin::ControllerPlugin(QObject* parent, IBioTrackerContext* context, ENUMS::CONTROLLERTYPE ctr) :
	IControllerCfg(parent, context, ctr) {
	m_BioTrackerPlugin = NULL;

	m_TrackingThread = new QThread(this);
   m_TrackingThread->setObjectName("TrackingThread");
	m_TrackingThread->start();
}

ControllerPlugin::~ControllerPlugin() {

	//They might not exist (no plugins available!)
	if(m_BioTrackerPlugin)
		delete m_BioTrackerPlugin;
	if(pluginLoader)
		delete pluginLoader;

	m_TrackingThread->quit();
	m_TrackingThread->wait();

}

void ControllerPlugin::addToPluginList(QString str) {

	std::string s = str.toStdString();
	pluginLoader->addToPluginList(str, TRACKER_SUFFIX);

	// Add Plugin name to Main Window
	IController* ctrA = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
	QPointer< ControllerMainWindow > ctrMainWindow = qobject_cast<ControllerMainWindow*>(ctrA);

	ctrMainWindow->setTrackerList(pluginLoader->getPluginList(),
		pluginLoader->getCurrentPluginName());
}

void ControllerPlugin::loadPluginFromFileName(QString str) {

	if (pluginLoader->loadPluginFromFilename(str)) {
		m_BioTrackerPlugin = qobject_cast<IBioTrackerPlugin*>(pluginLoader->getPluginInstance());
		createPlugin();

		// Add Plugin name to Main Window
		IController* ctrA = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
		QPointer< ControllerMainWindow > ctrMainWindow = qobject_cast<ControllerMainWindow*>(ctrA);

		ctrMainWindow->setTrackerList(pluginLoader->getPluginList(),
			pluginLoader->getCurrentPluginName());

		//Add Tracker Parameter to Main Window
		IView *parms = m_BioTrackerPlugin->getTrackerParameterWidget();
		ctrMainWindow->setTrackerParamterWidget(parms);

		//Add Tracker tracked components (Elements) to Main Window
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
	}else{
		qWarning() << "Failed to load plugin from filename!";
	}
}

void ControllerPlugin::selectPlugin(QString str) {
	if (str.isEmpty())
		return;

	if (pluginLoader->getCurrentPluginName() != str)
		loadPluginFromFileName(pluginLoader->getPluginMap().find(str)->second);
}

void ControllerPlugin::createModel() {
	m_Model = nullptr;
	pluginLoader = new PluginLoader(this);
}

void ControllerPlugin::createView() {
}

void ControllerPlugin::connectModelToController() {
}

void ControllerPlugin::loadPluginsFromPluginSubfolder() {

	//////////////
	QDir d(_cfg->DirPlugins);
	d.setFilter(QDir::Filter::Files);
	QStringList nameFilter;
	QFileInfoList fl = d.entryInfoList(nameFilter, QDir::Filter::Files);
	
	//Grab list of behaviors from config & registry
	std::vector<std::string> list = _cfg->UseRegistryLocations==1 ? PluginLoader::queryRegistryBehaviors(REGISTRY_PATH) : std::vector<std::string>();
	//Search directories
	std::vector<std::string> filesFromFolders = PluginLoader::searchDirectoriesForPlugins(list, TRACKER_SUFFIX);
	for(auto x: filesFromFolders)
		addToPluginList(x.c_str());

	foreach(QFileInfo fi, fl) {
		QString s = fi.absoluteFilePath();
		addToPluginList(s);
	}

	//Load plugins as per CLI
	if (!_cfg->UsePlugins.isEmpty()) {
		addToPluginList(_cfg->UsePlugins.toStdString().c_str());
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

	QObject::connect(this, SIGNAL(emitUpdateView()), ctrTrackedComponentCore,
		SLOT(receiveUpdateView()));

	// connect ControllerCommands
	IController* ctrD = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COMMANDS);
	QPointer< ControllerCommands > ctrCommands = qobject_cast<ControllerCommands*>(ctrD);

	QObject::connect(ctrCommands, SIGNAL(emitAddTrajectory(QPoint)), this,
		SLOT(receiveAddTrajectory(QPoint)), Qt::DirectConnection);
	QObject::connect(ctrCommands, SIGNAL(emitRemoveTrajectory(IModelTrackedTrajectory*)), this,
		SLOT(receiveRemoveTrajectory(IModelTrackedTrajectory*)), Qt::DirectConnection);
	QObject::connect(ctrCommands, SIGNAL(emitRemoveTrajectoryId(int)), this,
		SLOT(receiveRemoveTrajectoryId(int)), Qt::DirectConnection);
	QObject::connect(ctrCommands, SIGNAL(emitRemoveTrackEntity(IModelTrackedTrajectory*, uint)), this,
		SLOT(receiveRemoveTrackEntity(IModelTrackedTrajectory*, uint)), Qt::DirectConnection);
	QObject::connect(ctrCommands, SIGNAL(emitValidateTrajectory(int)), this,
		SLOT(receiveValidateTrajectory(int)), Qt::DirectConnection);
	QObject::connect(ctrCommands, SIGNAL(emitValidateEntity(IModelTrackedTrajectory*, uint)), this,
		SLOT(receiveValidateEntity(IModelTrackedTrajectory*, uint)), Qt::DirectConnection);
	QObject::connect(ctrCommands, SIGNAL(emitMoveElement(IModelTrackedTrajectory*, uint, QPoint, int)), this,
		SLOT(receiveMoveElement(IModelTrackedTrajectory*, uint, QPoint, int)), Qt::DirectConnection);
	QObject::connect(ctrCommands, SIGNAL(emitSwapIds(IModelTrackedTrajectory*, IModelTrackedTrajectory*)), this,
		SLOT(receiveSwapIds(IModelTrackedTrajectory*, IModelTrackedTrajectory*)), Qt::DirectConnection);
	QObject::connect(ctrCommands, SIGNAL(emitToggleFixTrack(IModelTrackedTrajectory*, bool)), this,
		SLOT(receiveToggleFixTrack(IModelTrackedTrajectory*, bool)), Qt::DirectConnection);
	QObject::connect(ctrCommands, SIGNAL(emitEntityRotation(IModelTrackedTrajectory*, double, uint)), this,
		SLOT(receiveEntityRotation(IModelTrackedTrajectory*, double, uint)), Qt::DirectConnection);


	// connect ControllerPlayer
	IController* ctrC = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
	QPointer< ControllerPlayer > ctrPlayer = qobject_cast<ControllerPlayer*>(ctrC);
	QObject::connect(ctrPlayer, SIGNAL(emitPauseState(bool)), this,
		SLOT(receivePauseState(bool)), Qt::DirectConnection);

	QObject::connect(ctrPlayer, SIGNAL(signalCurrentFrameNumberToPlugin(uint)),
		this, SLOT(receiveCurrentFrameNumberToPlugin(uint)), Qt::DirectConnection);

}

void ControllerPlugin::createPlugin() {
	m_BioTrackerPlugin = qobject_cast<IBioTrackerPlugin*>(pluginLoader->getPluginInstance());
	if (!m_BioTrackerPlugin){
		qFatal("Error at loading plugin.");
	}
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

	IController* ctrE = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
	QPointer< ControllerMainWindow > ctrMainWindow = qobject_cast<ControllerMainWindow*>(ctrE);

	QObject* obj = dynamic_cast<QObject*>(m_BioTrackerPlugin);

	QObject::connect(obj, SIGNAL(emitTrackingDone(uint)), ctDataEx, SLOT(receiveTrackingDone(uint)));

	QObject::connect(obj, SIGNAL(emitCvMat(std::shared_ptr<cv::Mat>, QString)),
		ctrTexture, SLOT(receiveCvMat(std::shared_ptr<cv::Mat>, QString)));

	QObject::connect(obj, SIGNAL(emitTrackingDone(uint)), ctrCompView, SLOT(receiveVisualizeTrackingModel(uint)));

	QObject::connect(obj, SIGNAL(emitChangeDisplayImage(QString)), ctrPlayer, SLOT(receiveChangeDisplayImage(QString)));

	QObject::connect(ctAreaDesc, SIGNAL(updateAreaDescriptor(IModelAreaDescriptor*)), obj, SLOT(receiveAreaDescriptor(IModelAreaDescriptor*)));

	QObject::connect(obj, SIGNAL(emitCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool>)), ctrCompView,
		SLOT(setCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool>)));

	QObject::connect(obj, SIGNAL(emitCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool>)), ctrCoreParam,
		SLOT(setCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool>)));

	QObject::connect(obj, SIGNAL(emitCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool>)), ctrMainWindow,
		SLOT(setCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool>)));

	QObject::connect(obj, SIGNAL(emitDimensionUpdate(int, int)), ctrCompView, SIGNAL(emitDimensionUpdate(int, int)));


	// data model actions
	QObject::connect(this, SIGNAL(emitRemoveTrajectory(IModelTrackedTrajectory*)), obj,
		SLOT(receiveRemoveTrajectory(IModelTrackedTrajectory*)));
	QObject::connect(this, SIGNAL(emitRemoveTrackEntity(IModelTrackedTrajectory*, uint)), obj,
		SIGNAL(emitRemoveTrackEntity(IModelTrackedTrajectory*, uint)));
	QObject::connect(this, SIGNAL(emitAddTrajectory(QPoint)), obj,
		SLOT(receiveAddTrajectory(QPoint)));
	QObject::connect(this, SIGNAL(emitMoveElement(IModelTrackedTrajectory*, uint, QPoint)), obj,
		SIGNAL(emitMoveElement(IModelTrackedTrajectory*, uint, QPoint)));
	QObject::connect(this, SIGNAL(emitSwapIds(IModelTrackedTrajectory*, IModelTrackedTrajectory*)), obj,
		SLOT(receiveSwapIds(IModelTrackedTrajectory*, IModelTrackedTrajectory*)));
	QObject::connect(this, SIGNAL(emitToggleFixTrack(IModelTrackedTrajectory*, bool)), obj,
		SIGNAL(emitToggleFixTrack(IModelTrackedTrajectory*, bool)));

	QObject::connect(this, SIGNAL(emitRemoveTrajectoryId(int)), obj,
		SIGNAL(emitRemoveTrajectoryId(int)));
	QObject::connect(this, SIGNAL(emitValidateTrajectory(int)), obj,
		SIGNAL(emitValidateTrajectory(int)));
	QObject::connect(this, SIGNAL(emitValidateEntity(IModelTrackedTrajectory*, uint)), obj,
		SIGNAL(emitValidateEntity(IModelTrackedTrajectory*, uint)));
	QObject::connect(this, SIGNAL(emitEntityRotation(IModelTrackedTrajectory*, double, uint)), obj,
		SIGNAL(emitEntityRotation(IModelTrackedTrajectory*, double, uint)));

	connect(this, &ControllerPlugin::frameRetrieved, m_BioTrackerPlugin, &IBioTrackerPlugin::receiveCurrentFrameFromMainApp);
	QObject::connect(this, SIGNAL(signalCurrentFrameNumberToPlugin(uint)), obj,
		SLOT(receiveCurrentFrameNumberFromMainApp(uint)));
}

void ControllerPlugin::disconnectPlugin() {

}

//first send all the commands currently in the command queue then the next image can be sent
void ControllerPlugin::sendCurrentFrameToPlugin(std::shared_ptr<cv::Mat> mat, uint number) {
	m_currentFrameNumber = number;

	//qDebug() << "Core send: " << number;

	//Prevent calling the plugin if none is loaded
	if (m_BioTrackerPlugin) {
		while (!m_editQueue.isEmpty()) {
			queueElement edit = m_editQueue.dequeue();

			switch (edit.type)
			{
			case EDIT::REMOVE_TRACK:
				emitRemoveTrajectory(edit.trajectory0);
				break;
			case EDIT::REMOVE_TRACK_ID:
				emitRemoveTrajectoryId(edit.id);
				break;
			case EDIT::REMOVE_ENTITY:
				emitRemoveTrackEntity(edit.trajectory0, edit.frameNumber);
				break;
			case EDIT::ADD:
				emitAddTrajectory(edit.pos);
				break;
			case EDIT::MOVE:
				emitMoveElement(edit.trajectory0, edit.frameNumber, edit.pos);
				break;
			case EDIT::SWAP:
				emitSwapIds(edit.trajectory0, edit.trajectory1);
				break;
			case EDIT::FIX:
				emitToggleFixTrack(edit.trajectory0, edit.toggle);
				break;
			case EDIT::VALIDATE:
				emitValidateTrajectory(edit.id);
				break;
			case EDIT::VALIDATE_ENTITY:
				emitValidateEntity(edit.trajectory0, edit.frameNumber);
				break;
			case  EDIT::ROTATE_ENTITY:
				emitEntityRotation(edit.trajectory0, edit.angle, edit.frameNumber);
				break;
			}
		}
		emit frameRetrieved(mat, number);
	}
}



//############################SLOTS##################################################

//  receivers of commands

void ControllerPlugin::receiveRemoveTrajectory(IModelTrackedTrajectory * trajectory)
{
	if (m_paused) {
		emitRemoveTrajectory(trajectory);
		emitUpdateView();
	}
	else {
		queueElement removeTrackEdit;
		removeTrackEdit.type = EDIT::REMOVE_TRACK;
		removeTrackEdit.trajectory0 = trajectory;
		m_editQueue.enqueue(removeTrackEdit);
	}
}

void ControllerPlugin::receiveRemoveTrajectoryId(int id)
{
	if (m_paused) {
		emitRemoveTrajectoryId(id);
		emitUpdateView();
	}
	else {
		queueElement removeTrackEdit;
		removeTrackEdit.type = EDIT::REMOVE_TRACK_ID;
		removeTrackEdit.id = id;
		m_editQueue.enqueue(removeTrackEdit);
	}
}

void ControllerPlugin::receiveRemoveTrackEntity(IModelTrackedTrajectory * trajectory, uint frameNumber)
{
	if (m_paused) {
		emitRemoveTrackEntity(trajectory, frameNumber);
		emitUpdateView();
	}
	else {
		queueElement removeEntityEdit;
		removeEntityEdit.type = EDIT::REMOVE_ENTITY;
		removeEntityEdit.trajectory0 = trajectory;
		removeEntityEdit.frameNumber = frameNumber;
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

void ControllerPlugin::receiveMoveElement(IModelTrackedTrajectory * trajectory, uint frameNumber, QPoint pos, int toMove)
{
	if (m_paused) {
		emitMoveElement(trajectory, frameNumber, pos);
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
		moveEdit.frameNumber = frameNumber;
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

void ControllerPlugin::receiveValidateTrajectory(int id)
{
	if (m_paused) {
		emitValidateTrajectory(id);
		emitUpdateView();
	}
	else {
		queueElement validateEdit;
		validateEdit.type = EDIT::VALIDATE;
		validateEdit.id = id;
		m_editQueue.enqueue(validateEdit);
	}
}

void ControllerPlugin::receiveValidateEntity(IModelTrackedTrajectory * trajectory, uint frameNumber)
{
	if (m_paused) {
		emitValidateEntity(trajectory, frameNumber);
		emitUpdateView();
	}
	else {
		queueElement validateEntityEdit;
		validateEntityEdit.type = EDIT::VALIDATE_ENTITY;
		validateEntityEdit.trajectory0 = trajectory;
		validateEntityEdit.frameNumber = frameNumber;
		m_editQueue.enqueue(validateEntityEdit);
	}
}

void ControllerPlugin::receiveToggleFixTrack(IModelTrackedTrajectory * trajectory, bool toggle)
{
	if (m_paused) {
		emitToggleFixTrack(trajectory, toggle);
		emitUpdateView();
	}
	else {
		queueElement fixEdit;
		fixEdit.type = EDIT::FIX;
		fixEdit.trajectory0 = trajectory;
		fixEdit.toggle = toggle;
		m_editQueue.enqueue(fixEdit);
	}
}

void ControllerPlugin::receiveEntityRotation(IModelTrackedTrajectory * trajectory, double angle, uint frameNumber)
{
	if (m_paused) {
		emitEntityRotation(trajectory, angle, frameNumber);
		emitUpdateView();
	}
	else {
		queueElement rotEdit;
		rotEdit.type = EDIT::ROTATE_ENTITY;
		rotEdit.trajectory0 = trajectory;
		rotEdit.angle = angle;
		rotEdit.frameNumber = frameNumber;
		m_editQueue.enqueue(rotEdit);
	}
}

//*********************************************************************

void ControllerPlugin::receivePauseState(bool state)
{
	m_paused = state;
}

void ControllerPlugin::receiveCurrentFrameNumberToPlugin(uint frameNumber)
{
	m_currentFrameNumber = frameNumber;
	Q_EMIT signalCurrentFrameNumberToPlugin(frameNumber);
}

void ControllerPlugin::receiveTrackingDone() {

}

