#include "BioTrackerPlugin.h"

#include "Controller/ControllerTextureObject.h"
#include "Controller/ControllerPlayer.h"


BioTrackerPlugin::BioTrackerPlugin()
{
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(BioTrackerPlugin, BioTrackerPlugin)
#endif // QT_VERSION < 0x050000


void BioTrackerPlugin::createComponents()
{
    createModel();
    createView();
    connectModelController();
}

void BioTrackerPlugin::connectComponents()
{
    connectController();
}

void BioTrackerPlugin::addView(IView *view)
{
    m_View = view;
}

void BioTrackerPlugin::addModel(IModel *model)
{
    m_Model = model;
}

IModel *BioTrackerPlugin::getModel()
{
    return m_Model;
}

IView *BioTrackerPlugin::getView()
{
    return m_View;
}

ENUMS::CONTROLLERTYPE BioTrackerPlugin::getControllerType()
{
    return ENUMS::CONTROLLERTYPE::TRACKING;
}

IBioTrackerContext *BioTrackerPlugin::getBioTrackerContext()
{
    return m_BioTrackerContext;
}

void BioTrackerPlugin::createModel()
{
    m_Model = new BioTrackerTrackingAlgorithm();
}

void BioTrackerPlugin::createView()
{
}

void BioTrackerPlugin::connectModelController()
{
}

void BioTrackerPlugin::connectController()
{
//    IController *ctrA = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
//    ControllerTextureObject *ctrTexture = dynamic_cast<ControllerTextureObject *>(ctrA);
//    QObject::connect(this, &BioTrackerPlugin::emitCvMatA, ctrTexture, &ControllerTextureObject::receiveCvMat);


//    IController *ctrB = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
//    ControllerPlayer *ctrPlayer = dynamic_cast<ControllerPlayer *>(ctrB);
//    IModel *model = ctrPlayer->getModel();
//    BioTracker3Player *player = dynamic_cast<BioTracker3Player *>(model);

//    BioTrackerTrackingAlgorithm *alg = qobject_cast<BioTrackerTrackingAlgorithm *>(m_Model);
//    QObject::connect(player, &BioTracker3Player::emitCurrentFrame, alg, &BioTrackerTrackingAlgorithm::doTracking);

//    QObject::connect(alg, &BioTrackerTrackingAlgorithm::emitCvMatA, ctrTexture, &ControllerTextureObject::receiveCvMat);
}

void BioTrackerPlugin::receiveCvMatA(std::shared_ptr<cv::Mat> mat, QString name)
{
    Q_EMIT emitCvMatA(mat, name);
}


void BioTrackerPlugin::addBioTrackerContext(IBioTrackerContext *context)
{
    m_BioTrackerContext = context;
}
