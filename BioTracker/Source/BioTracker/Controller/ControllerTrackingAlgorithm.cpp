#include "ControllerTrackingAlgorithm.h"
#include "ControllerTextureObject.h"
#include "ControllerPlayer.h"

ControllerTrackingAlgorithm::ControllerTrackingAlgorithm(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr)
{

}

void ControllerTrackingAlgorithm::connectControllerToController()
{
    IController *ctrA = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TEXTUREOBJECT);
    ControllerTextureObject *ctrTexture = dynamic_cast<ControllerTextureObject *>(ctrA);
    QObject::connect(this, &ControllerTrackingAlgorithm::emitCvMatA, ctrTexture, &ControllerTextureObject::receiveCvMat);


    IController *ctrB = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    ControllerPlayer *ctrPlayer = dynamic_cast<ControllerPlayer *>(ctrB);
    IModel *model = ctrPlayer->getModel();
    BioTracker3Player *player = dynamic_cast<BioTracker3Player *>(model);

    BioTrackerTrackingAlgorithm *alg = qobject_cast<BioTrackerTrackingAlgorithm *>(m_Model);
    QObject::connect(player, SIGNAL(emitCurrentFrame(std::shared_ptr<cv::Mat>)), alg, SLOT(doTracking(std::shared_ptr<cv::Mat>)));

    QObject::connect(alg, &BioTrackerTrackingAlgorithm::emitCvMatA, ctrTexture, &ControllerTextureObject::receiveCvMat);
}

void ControllerTrackingAlgorithm::createModel()
{
    m_Model = new BioTrackerTrackingAlgorithm();
}

void ControllerTrackingAlgorithm::createView()
{

}

void ControllerTrackingAlgorithm::connectModelToController()
{

}

void ControllerTrackingAlgorithm::receiveCvMatA(std::shared_ptr<cv::Mat> mat, QString name)
{
    Q_EMIT emitCvMatA(mat, name);
}
