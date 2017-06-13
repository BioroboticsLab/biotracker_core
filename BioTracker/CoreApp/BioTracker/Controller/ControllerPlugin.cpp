#include "ControllerPlugin.h"

#include "Controller/ControllerPlayer.h"
#include "Controller/ControllerTextureObject.h"
#include "ControllerMainWindow.h"
#include "QDebug"
#include "Model/PluginLoader.h"

ControllerPlugin::ControllerPlugin(QObject* parent, IBioTrackerContext* context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr) {
	m_BioTrackerPlugin = NULL;//TODO Andi init properly!

    m_TrackingThread = new QThread(this);
    m_TrackingThread->start();
}

ControllerPlugin::~ControllerPlugin() {
    m_TrackingThread->quit();
    m_TrackingThread->wait();

}

void ControllerPlugin::loadPluginFromFileName(QString str) {
    PluginLoader* loader = qobject_cast<PluginLoader*>(m_Model);
    if( loader->loadPluginFromFilename(str)) {
        createPlugin();

        // Add Plugin name to Main Window
        IController* ctrA = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
        QPointer< ControllerMainWindow > ctrMainWindow = qobject_cast<ControllerMainWindow*>(ctrA);

        ctrMainWindow->setTrackerList(qobject_cast<PluginLoader*>(m_Model)->getPluginMetaData());

        //Add Tracker Parameter to Main Window
        ctrMainWindow->setTrackerParamterWidget(m_BioTrackerPlugin->getTrackerParameterWidget());

		//Add Tracker tracked components (Elements) to Main Window
		IView *v = m_BioTrackerPlugin->getTrackerElementsWidget();
		ctrMainWindow->setTrackerElementsWidget(m_BioTrackerPlugin->getTrackerElementsWidget()); //MARKER
    }
}

void ControllerPlugin::createModel() {
    m_Model = new PluginLoader(this);
}

void ControllerPlugin::createView() {
}

void ControllerPlugin::connectModelToController() {
}

void ControllerPlugin::connectControllerToController() {

    // Add Plugin name to Main Window
    IController* ctrA = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
    QPointer< ControllerMainWindow > ctrMainWindow = qobject_cast<ControllerMainWindow*>(ctrA);

    ctrMainWindow->deactiveTrackringCheckBox();
}

void ControllerPlugin::createPlugin() {
    m_BioTrackerPlugin = qobject_cast<PluginLoader*>(m_Model)->getPluginInstance();
    m_BioTrackerPlugin->createPlugin();

    m_BioTrackerPlugin->moveToThread(m_TrackingThread);

    connectPlugin();
}

void ControllerPlugin::connectPlugin() {
    IController* ctrA = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    QPointer< ControllerPlayer > ctrPlayer = qobject_cast<ControllerPlayer*>(ctrA);

    IModel* model = ctrPlayer->getModel();

    IController* ctrB = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
    ControllerTextureObject* ctrTexture = qobject_cast<ControllerTextureObject*>(ctrB);

    QObject* obj = dynamic_cast<QObject*>(m_BioTrackerPlugin);

    QObject::connect(obj, SIGNAL(emitCvMat(std::shared_ptr<cv::Mat>, QString)),
                     ctrTexture, SLOT(receiveCvMat(std::shared_ptr<cv::Mat>, QString)));

    QObject::connect(obj, SIGNAL(emitTrackingDone()), ctrPlayer, SLOT(receiveTrackingOperationDone()));
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

void ControllerPlugin::sendCurrentFrameToPlugin(std::shared_ptr<cv::Mat> mat, uint number) {
	//Prevent calling the plugin if none is loaded
	if(m_BioTrackerPlugin)
		m_BioTrackerPlugin->receiveCurrentFrameFromMainApp(mat, number);
}

void ControllerPlugin::receiveTrackingDone() {

}
