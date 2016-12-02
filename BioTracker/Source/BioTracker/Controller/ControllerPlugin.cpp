#include "ControllerPlugin.h"

#include "Controller/ControllerPlayer.h"
#include "Controller/ControllerTextureObject.h"
#include "QDebug"

ControllerPlugin::ControllerPlugin(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr)
{

}

void ControllerPlugin::loadPlugin(QString str)
{
    if(m_PluginLoader->isLoaded()) {
        disconnectPlugin();
        m_PluginLoader->unload();
    }

    m_PluginLoader->setFileName(str);

    bool x = QLibrary::isLibrary(str);

    QObject *plugin = m_PluginLoader->instance();
    QString sb = m_PluginLoader->errorString();
    qDebug() << sb;

    if (plugin) {
        m_BioTrackerPlugin = qobject_cast<IBioTrackerPlugin *>(plugin);
        if (m_BioTrackerPlugin) {
            m_BioTrackerPlugin->createPlugin();

            connectPlugin();
        }
    }
}


void ControllerPlugin::createModel()
{
    m_PluginLoader = new QPluginLoader(this);
}

void ControllerPlugin::createView()
{
}

void ControllerPlugin::connectModelController()
{
}

void ControllerPlugin::connectController()
{

}

void ControllerPlugin::connectPlugin()
{
    IController * ctrA = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    QPointer< ControllerPlayer > ctrPlayer = qobject_cast<ControllerPlayer *>(ctrA);

    IModel *model = ctrPlayer->getModel();
    BioTracker3Player *player = dynamic_cast<BioTracker3Player *>(model);


    IController * ctrB = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
    ControllerTextureObject *ctrTexture = qobject_cast<ControllerTextureObject *>(ctrB);

    QObject *obj = dynamic_cast<QObject *>(m_BioTrackerPlugin);

    QObject::connect(dynamic_cast<QObject*> (m_BioTrackerPlugin), SIGNAL(emitCvMat(std::shared_ptr<cv::Mat>,QString)),
                     ctrTexture, SLOT(receiveCvMat(std::shared_ptr<cv::Mat>,QString)));
    QObject::connect(player, &BioTracker3Player::emitCurrentFrame, this, &ControllerPlugin::receiveCurrentFrame);
}

void ControllerPlugin::disconnectPlugin()
{
    IController * ctrA = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    QPointer< ControllerPlayer > ctrPlayer = qobject_cast<ControllerPlayer *>(ctrA);

    IModel *model = ctrPlayer->getModel();
    BioTracker3Player *player = dynamic_cast<BioTracker3Player *>(model);


    IController * ctrB = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
    QPointer< ControllerTextureObject > ctrTexture = qobject_cast<ControllerTextureObject *>(ctrB);


//    QObject::disconnect(qobject_cast<QObject*> (m_BioTrackerPlugin), SIGNAL(emitCvMat(std::shared_ptr<cv::Mat>,QString)), ctrTexture, SLOT(receiveCvMat(std::shared_ptr<cv::Mat>,QString)));


//    QObject::disconnect(player, SIGNAL(emitCurrentFrame(std::shared_ptr<cv::Mat>)), qobject_cast<QObject*> (m_BioTrackerPlugin), SLOT(receiveCvMat(std::shared_ptr<cv::Mat>)));

}

void ControllerPlugin::receiveCurrentFrame(std::shared_ptr<cv::Mat> mat)
{
    m_BioTrackerPlugin->receiveCvMat(mat);
}
