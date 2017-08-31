#include "ControllerMainWindow.h"
#include "View/MainWindow.h"
#include "Model/null_Model.h"
//#include "Controller/ControllerStrategies/MainGUIApplication.h"
#include "Controller/ControllerPlayer.h"
#include "Controller/ControllerPlugin.h"
#include "GuiContext.h"

#include "QPluginLoader"
#include "util/types.h"


ControllerMainWindow::ControllerMainWindow(QObject* parent, IBioTrackerContext* context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr) {

}

void ControllerMainWindow::loadVideo(QString str) {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer*>(ctr)->loadVideoStream(str);
}

void ControllerMainWindow::loadTracker(QString str) {
    qobject_cast<GuiContext*>(m_BioTrackerContext)->loadBioTrackerPlugin(str);

}

void ControllerMainWindow::loadPictures(std::vector<boost::filesystem::path> files) {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer*>(ctr)->loadPictures(files);
}

void ControllerMainWindow::loadCameraDevice(CameraConfiguration conf) {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer*>(ctr)->loadCameraDevice(conf);
}

void ControllerMainWindow::activeTracking() {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer*>(ctr)->setTrackingActivated();
}

void ControllerMainWindow::deactiveTrackring() {
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    qobject_cast<ControllerPlayer*>(ctr)->setTrackingDeactivated();
}

void ControllerMainWindow::setTrackerList(QStringListModel* trackerList) {
    dynamic_cast<MainWindow*>(m_View)->setTrackerList(trackerList);
}

void ControllerMainWindow::setTrackerParamterWidget(IView* widget) {
    dynamic_cast<MainWindow*>(m_View)->addTrackerParameterView(widget);
}

void ControllerMainWindow::setTrackerElementsWidget(IView *widget)
{
	dynamic_cast<MainWindow*>(m_View)->addTrackerElementsView(widget);
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
	
}

void ControllerMainWindow::connectControllerToController() {

}

void ControllerMainWindow::rcvSelectPlugin(QString plugin) {
	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLUGIN);
	qobject_cast<ControllerPlugin*>(ctr)->selectPlugin(plugin);
}


