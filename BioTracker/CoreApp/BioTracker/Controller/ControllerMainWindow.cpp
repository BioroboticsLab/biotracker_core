#include "ControllerMainWindow.h"
#include "View/MainWindow.h"
#include "Model/null_Model.h"
#include "Model/Annotations.h"
//#include "Controller/ControllerStrategies/MainGUIApplication.h"
#include "Controller/ControllerPlayer.h"
#include "Controller/ControllerPlugin.h"
#include "Controller/ControllerAnnotations.h"
#include "Controller/ControllerDataExporter.h"
#include "Controller/ControllerTrackedComponentCore.h"
#include "GuiContext.h"

#include "QPluginLoader"
#include "util/types.h"


ControllerMainWindow::ControllerMainWindow(QObject* parent, IBioTrackerContext* context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr) {

}

void ControllerMainWindow::loadVideo(QString str) {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer*>(ctr)->loadVideoStream(str);
    onNewMediumLoaded(str.toStdString());
}

void ControllerMainWindow::loadTracker(QString str) {
    qobject_cast<GuiContext*>(m_BioTrackerContext)->loadBioTrackerPlugin(str);

}

void ControllerMainWindow::loadPictures(std::vector<boost::filesystem::path> files) {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer*>(ctr)->loadPictures(files);
    onNewMediumLoaded(files.empty() ? "" : files.front().string());
}

void ControllerMainWindow::loadCameraDevice(CameraConfiguration conf) {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer*>(ctr)->loadCameraDevice(conf);
    onNewMediumLoaded("::Camera");
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
    QObject::connect(this, &ControllerMainWindow::emitMediaLoaded, annotationController, &ControllerAnnotations::reset);

    // Write previously written data structure and reset it
    IController* ctr2 = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::DATAEXPORT);
    ControllerDataExporter *contrl = static_cast<ControllerDataExporter*>(ctr2);
    QObject::connect(this, &ControllerMainWindow::emitMediaLoaded, contrl, &ControllerDataExporter::receiveFinalizeExperiment);
    QObject::connect(this, &ControllerMainWindow::emitPluginLoaded, contrl, &ControllerDataExporter::receiveReset);
}

void ControllerMainWindow::connectControllerToController() {

	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COREPARAMETER);
	IView* v = ctr->getView();
	dynamic_cast<MainWindow*>(m_View)->addCoreParameterView(v);

	BioTracker::Core::Settings *set = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
	std::string *video = (std::string*)(set->readValue("video"));
	if (video)
		loadVideo(video->c_str());
}

void ControllerMainWindow::rcvSelectPlugin(QString plugin) {
	Q_EMIT emitPluginLoaded(plugin.toStdString());
	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLUGIN);
	qobject_cast<ControllerPlugin*>(ctr)->selectPlugin(plugin);
}

void ControllerMainWindow::onNewMediumLoaded(const std::string path)
{
    Q_EMIT emitMediaLoaded(path);
}
