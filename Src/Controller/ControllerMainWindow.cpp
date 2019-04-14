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
#include "Controller/ControllerCoreParameter.h"
#include "GuiContext.h"

#include "QPluginLoader"

#include <chrono>
#include <thread>


ControllerMainWindow::ControllerMainWindow(QObject* parent, IBioTrackerContext* context, ENUMS::CONTROLLERTYPE ctr) :
    IControllerCfg(parent, context, ctr) {

}

void ControllerMainWindow::loadVideo(std::vector<boost::filesystem::path> files) {

    Q_EMIT emitOnLoadMedia(files.front().string());
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer*>(ctr)->loadVideoStream(files);
    Q_EMIT emitMediaLoaded(files.front().string());
    qDebug() << "CORE:  Video loaded: " << QString::fromStdString(files.front().string());

    Q_EMIT emitFilesCount((int)files.size());
    
    dynamic_cast<MainWindow*>(m_View)->checkMediaGroupBox();
}

void ControllerMainWindow::loadTracker(QString plugin) {
    Q_EMIT emitOnLoadPlugin(plugin.toStdString());
    dynamic_cast<MainWindow*>(m_View)->resetTrackerViews();
    qobject_cast<GuiContext*>(m_BioTrackerContext)->loadBioTrackerPlugin(plugin);
    dynamic_cast<MainWindow*>(m_View)->checkTrackerGroupBox();
    activeTrackingCheckBox();
    Q_EMIT emitPluginLoaded(plugin.toStdString());
}

void ControllerMainWindow::loadPictures(std::vector<boost::filesystem::path> files) {
    std::string str = files.empty() ? "" : files.front().string();
    
    Q_EMIT emitOnLoadMedia(str);
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer*>(ctr)->loadPictures(files);
    Q_EMIT emitMediaLoaded(str);

    qDebug() << "CORE:  Picture loaded: " << QString::fromStdString(files.front().string());

    Q_EMIT emitFilesCount((int)files.size());

    dynamic_cast<MainWindow*>(m_View)->checkMediaGroupBox();
}

void ControllerMainWindow::loadCameraDevice(CameraConfiguration conf) {
    Q_EMIT emitOnLoadMedia("::Camera");
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer*>(ctr)->loadCameraDevice(conf);
    Q_EMIT emitMediaLoaded("::Camera");

    qDebug() << "CORE:  Camera loaded!";

    Q_EMIT emitFilesCount(1);

    dynamic_cast<MainWindow*>(m_View)->checkMediaGroupBox();
}

void ControllerMainWindow::activeTracking() {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer*>(ctr)->setTrackingActivated();
    dynamic_cast<MainWindow*>(m_View)->activateTracking();
}

void ControllerMainWindow::deactiveTrackring() {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer*>(ctr)->setTrackingDeactivated();
    dynamic_cast<MainWindow*>(m_View)->deactivateTracking();
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

void ControllerMainWindow::saveTrajectoryFile(std::string file) {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::DATAEXPORT);
    static_cast<ControllerDataExporter*>(ctr)->saveFile(file);
}

void ControllerMainWindow::deactiveTrackingCheckBox() {
    dynamic_cast<MainWindow*>(m_View)->deactivateTrackingCheckBox();
}

void ControllerMainWindow::activeTrackingCheckBox() {
    dynamic_cast<MainWindow*>(m_View)->activeTrackingCheckBox();
}

void ControllerMainWindow::receiveSaveTrajData(){
    dynamic_cast<MainWindow*>(m_View)->saveDataToFile();
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
	QObject::connect(this, &ControllerMainWindow::emitShowActionListCommand, ctrcmd, &ControllerCommands::receiveShowActionList, Qt::DirectConnection);

	//connect to ControllerGraphicScene
	IController* ictrgrv = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);
	ControllerGraphicScene *ctrgrv = static_cast<ControllerGraphicScene*>(ictrgrv);
	GraphicsView* grv = static_cast<GraphicsView*>(ctrgrv->getView());
	
	QObject::connect(grv, &GraphicsView::emitCursorPosition, this, &ControllerMainWindow::receiveCursorPosition, Qt::QueuedConnection);

    //connect to ControllerTrackedComponentCore
    IController* ictrtrcc = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE);
	ControllerTrackedComponentCore *ctrtrcc = static_cast<ControllerTrackedComponentCore*>(ictrtrcc);

    QObject::connect(this, &ControllerMainWindow::emitAddTrack, ctrtrcc, &ControllerTrackedComponentCore::emitAddTrack, Qt::DirectConnection);
	QObject::connect(this, &ControllerMainWindow::emitDeleteSelectedTracks, ctrtrcc, &ControllerTrackedComponentCore::emitDeleteSelectedTracks, Qt::DirectConnection);
	QObject::connect(this, &ControllerMainWindow::emitSelectAll, ctrtrcc, &ControllerTrackedComponentCore::emitSelectAll, Qt::DirectConnection);
 	QObject::connect(this, &ControllerMainWindow::emitSwapIds, ctrtrcc, &ControllerTrackedComponentCore::emitSwapIdsToView, Qt::DirectConnection);   
    QObject::connect(this, &ControllerMainWindow::emitChangeColorBorder, ctrtrcc, &ControllerTrackedComponentCore::emitChangeColorBorder, Qt::DirectConnection);
	QObject::connect(this, &ControllerMainWindow::emitChangeColorFill, ctrtrcc, &ControllerTrackedComponentCore::emitChangeColorFill, Qt::DirectConnection);
	
    //connect to ControllerAnnotations
    IController* ictrann = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::ANNOTATIONS);
	ControllerAnnotations *ctrann = static_cast<ControllerAnnotations*>(ictrann);

    QObject::connect(this, &ControllerMainWindow::emitAddLabelAnno, ctrann, &ControllerAnnotations::receiveAddLabelAnnotation, Qt::DirectConnection);
	QObject::connect(this, &ControllerMainWindow::emitAddRectAnno, ctrann, &ControllerAnnotations::receiveAddRectAnnotation, Qt::DirectConnection);
	QObject::connect(this, &ControllerMainWindow::emitAddArrAnno, ctrann, &ControllerAnnotations::receiveAddArrowAnnotation, Qt::DirectConnection);
	QObject::connect(this, &ControllerMainWindow::emitAddEllAnno, ctrann, &ControllerAnnotations::receiveAddEllipseAnnotation, Qt::DirectConnection);
	QObject::connect(this, &ControllerMainWindow::emitDelSelAnno, ctrann, &ControllerAnnotations::receiveDeleteSelectedAnnotation, Qt::DirectConnection);

    //connect to ControllerDataExporter
    IController* ictrde = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::DATAEXPORT);
    ControllerDataExporter *deController = static_cast<ControllerDataExporter*>(ictrde);
    QObject::connect(this, &ControllerMainWindow::emitFinalizeExperiment, deController, &ControllerDataExporter::receiveFinalizeExperiment, Qt::DirectConnection);

	// reset trials
	IController* ictrcpv = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COREPARAMETER);
	ControllerCoreParameter *ctrcpv = static_cast<ControllerCoreParameter*>(ictrcpv);
	QObject::connect(this, &ControllerMainWindow::emitOnLoadMedia, ctrcpv, &ControllerCoreParameter::receiveResetTrial, Qt::DirectConnection);
	QObject::connect(this, &ControllerMainWindow::emitOnLoadPlugin, ctrcpv, &ControllerCoreParameter::receiveResetTrial, Qt::DirectConnection);

    // batch media signals
    IController* ctr3 = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    ControllerPlayer *cont3 = static_cast<ControllerPlayer*>(ctr3);
	QObject::connect(cont3, &ControllerPlayer::emitNextMediaInBatch, this, &ControllerMainWindow::emitOnLoadMedia, Qt::DirectConnection);
	QObject::connect(cont3, &ControllerPlayer::emitNextMediaInBatchLoaded, this, &ControllerMainWindow::emitMediaLoaded, Qt::DirectConnection);

	//Load video as per CLI
    if (!_cfg->LoadVideo.isEmpty()) 
        loadVideo({ _cfg->LoadVideo.toStdString().c_str() });
}

void ControllerMainWindow::receiveCursorPosition(QPoint pos)
{
	//qDebug() << pos;
	dynamic_cast<MainWindow*>(m_View)->setCursorPositionLabel(pos);
}

void ControllerMainWindow::setCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool> permission){
    if( dynamic_cast<MainWindow*>(m_View)){
        dynamic_cast<MainWindow*>(m_View)->setCorePermission(permission);
    }
    else{
        assert(false);
    }
}

void ControllerMainWindow::rcvSelectPlugin(QString plugin) {
    Q_EMIT emitOnLoadPlugin(plugin.toStdString());
    dynamic_cast<MainWindow*>(m_View)->resetTrackerViews();
	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLUGIN);
	qobject_cast<ControllerPlugin*>(ctr)->selectPlugin(plugin);
	//dynamic_cast<MainWindow*>(m_View)->activeTrackingCheckBox();
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
	delete m_BioTrackerContext;
}

