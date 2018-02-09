#include "ControllerMainWindow.h"
#include "View/MainWindow.h"
#include "View/GraphicsView.h"
#include "Model/null_Model.h"
#include "Model/Annotations.h"

#include "Controller/ControllerPlayer.h"
#include "Controller/ControllerPlugin.h"
#include "Controller/ControllerAnnotations.h"
#include "Controller/ControllerDataExporter.h"
#include "Controller/ControllerTrackedComponentCore.h"
#include "Controller/ControllerCommands.h"
#include "Controller/ControllerGraphicScene.h"
#include "Controller/ControllerGraphicScene.h"
#include "GuiContext.h"

#include "QPluginLoader"
#include "util/types.h"


ControllerMainWindow::ControllerMainWindow(QObject* parent, IBioTrackerContext* context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr) {

}

void ControllerMainWindow::loadVideo(QString str) {

    Q_EMIT emitOnLoadMedia(str.toStdString());
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer*>(ctr)->loadVideoStream(str);
    Q_EMIT emitMediaLoaded(str.toStdString());
}

void ControllerMainWindow::loadTracker(QString str) {
    qobject_cast<GuiContext*>(m_BioTrackerContext)->loadBioTrackerPlugin(str);

}

void ControllerMainWindow::loadPictures(std::vector<boost::filesystem::path> files) {
    std::string str = files.empty() ? "" : files.front().string();
    
    Q_EMIT emitOnLoadMedia(str);
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer*>(ctr)->loadPictures(files);
    Q_EMIT emitMediaLoaded(str);
}

void ControllerMainWindow::loadCameraDevice(CameraConfiguration conf) {
    Q_EMIT emitOnLoadMedia("::Camera");
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer*>(ctr)->loadCameraDevice(conf);
    Q_EMIT emitMediaLoaded("::Camera");
}

void ControllerMainWindow::activeTracking() {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer*>(ctr)->setTrackingActivated();
}

void ControllerMainWindow::deactiveTrackring() {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer*>(ctr)->setTrackingDeactivated();
}

void ControllerMainWindow::setTrackerList(QStringListModel* trackerList, QString current) {
    dynamic_cast<MainWindow*>(m_View)->setTrackerList(trackerList, current);
}

void ControllerMainWindow::setTrackerParamterWidget(IView* widget) {
    dynamic_cast<MainWindow*>(m_View)->addTrackerParameterView(widget);
}

void ControllerMainWindow::setTrackerElementsWidget(IView *widget)
{
	dynamic_cast<MainWindow*>(m_View)->addTrackerElementsView(widget);
}

void ControllerMainWindow::setCoreElementsWidget(IView * widget)
{
	dynamic_cast<MainWindow*>(m_View)->addCoreElementsView(widget);
}

void ControllerMainWindow::setNotificationBrowserWidget(IView* widget)
{
	dynamic_cast<MainWindow*>(m_View)->addNotificationBrowser(widget);
}


void ControllerMainWindow::loadTrajectoryFile(std::string file) {
	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::DATAEXPORT);
	static_cast<ControllerDataExporter*>(ctr)->loadFile(file);
    Q_EMIT emitTrackLoaded(file);
}

void ControllerMainWindow::deactiveTrackingCheckBox() {
    dynamic_cast<MainWindow*>(m_View)->deactivateTrackingCheckBox();
}

void ControllerMainWindow::activeTrackingCheckBox() {
    dynamic_cast<MainWindow*>(m_View)->activeTrackingCheckBox();
}

void ControllerMainWindow::createModel()
{
    // no model for MainWindow
    m_Model = new null_Model();
}

void ControllerMainWindow::createView() {
    m_View = dynamic_cast<IView*> (new MainWindow(0, this, m_Model));
    static_cast<MainWindow*>(m_View)->show();
}

void ControllerMainWindow::connectModelToController() {

	MainWindow *mw = dynamic_cast<MainWindow*> (m_View);
	QObject::connect(mw, &MainWindow::selectPlugin, this, &ControllerMainWindow::rcvSelectPlugin);
	
    // Prepare annotations and serialize existing ones.
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::ANNOTATIONS);
    ControllerAnnotations *annotationController = static_cast<ControllerAnnotations*>(ctr);
    QObject::connect(this, &ControllerMainWindow::emitMediaLoaded, annotationController, &ControllerAnnotations::reset, Qt::DirectConnection);

    // Write previously written data structure and reset it
    IController* ctr2 = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::DATAEXPORT);
    ControllerDataExporter *contrl = static_cast<ControllerDataExporter*>(ctr2);
    QObject::connect(this, &ControllerMainWindow::emitOnLoadMedia, contrl, &ControllerDataExporter::receiveFinalizeExperiment, Qt::DirectConnection);
    QObject::connect(this, &ControllerMainWindow::emitOnLoadPlugin, contrl, &ControllerDataExporter::receiveReset, Qt::DirectConnection);

}

void ControllerMainWindow::connectControllerToController() {
	//
	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COREPARAMETER);
	IView* v = ctr->getView();
	dynamic_cast<MainWindow*>(m_View)->addCoreParameterView(v);

	//connect to controller commands
	IController* ictrcmd = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COMMANDS);
	ControllerCommands *ctrcmd = static_cast<ControllerCommands*>(ictrcmd);

	QObject::connect(this, &ControllerMainWindow::emitUndoCommand, ctrcmd, &ControllerCommands::receiveUndo, Qt::DirectConnection);
	QObject::connect(this, &ControllerMainWindow::emitRedoCommand, ctrcmd, &ControllerCommands::receiveRedo, Qt::DirectConnection);
	QObject::connect(this, &ControllerMainWindow::emitOnLoadMedia, ctrcmd, &ControllerCommands::receiveClear, Qt::DirectConnection);
	QObject::connect(this, &ControllerMainWindow::emitOnLoadPlugin, ctrcmd, &ControllerCommands::receiveClear, Qt::DirectConnection);
    //QObject::connect(this, &ControllerMainWindow::emit, ctrcmd, &ControllerCommands::receiveClear, Qt::DirectConnection);
	QObject::connect(this, &ControllerMainWindow::emitShowActionListCommand, ctrcmd, &ControllerCommands::receiveShowActionList, Qt::DirectConnection);

	//connect to ControllerGraphicScene
	IController* ictrgrv = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);
	ControllerGraphicScene *ctrgrv = static_cast<ControllerGraphicScene*>(ictrgrv);
	GraphicsView* grv = static_cast<GraphicsView*>(ctrgrv->getView());
	
	QObject::connect(grv, &GraphicsView::emitCursorPosition, this, &ControllerMainWindow::receiveCursorPosition, Qt::QueuedConnection);


	//
	BioTracker::Core::Settings *set = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
	std::string *video = (std::string*)(set->readValue("video"));
	if (video)
		loadVideo(video->c_str());
}

void ControllerMainWindow::receiveCursorPosition(QPoint pos)
{
	//qDebug() << pos;
	dynamic_cast<MainWindow*>(m_View)->setCursorPositionLabel(pos);
}

void ControllerMainWindow::rcvSelectPlugin(QString plugin) {
    Q_EMIT emitOnLoadPlugin(plugin.toStdString());
	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLUGIN);
	qobject_cast<ControllerPlugin*>(ctr)->selectPlugin(plugin);
	dynamic_cast<MainWindow*>(m_View)->activeTrackingCheckBox();
	activeTrackingCheckBox();
    Q_EMIT emitPluginLoaded(plugin.toStdString());
}

void ControllerMainWindow::onNewMediumLoaded(const std::string path)
{
    Q_EMIT emitMediaLoaded(path);
}

void ControllerMainWindow::exit() {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    ControllerPlayer *pc = static_cast<ControllerPlayer*>(ctr);
    pc->stop();
    Sleep(1);
	delete m_BioTrackerContext;
}

