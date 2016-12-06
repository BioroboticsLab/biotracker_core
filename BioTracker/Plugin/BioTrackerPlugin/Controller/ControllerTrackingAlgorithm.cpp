#include "ControllerTrackingAlgorithm.h"

ControllerTrackingAlgorithm::ControllerTrackingAlgorithm(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr)
{

}

void ControllerTrackingAlgorithm::connectController()
{

}

void ControllerTrackingAlgorithm::doTracking(std::shared_ptr<cv::Mat> mat, uint number)
{
    qobject_cast<BioTrackerTrackingAlgorithm *>(m_Model)->doTracking(mat, number);
}

void ControllerTrackingAlgorithm::createModel()
{
    m_Model = new BioTrackerTrackingAlgorithm();
}

void ControllerTrackingAlgorithm::createView()
{

}

void ControllerTrackingAlgorithm::connectModelController()
{
    BioTrackerTrackingAlgorithm *trackingAlg = qobject_cast<BioTrackerTrackingAlgorithm *>(m_Model);
    QObject::connect(trackingAlg, &BioTrackerTrackingAlgorithm::emitCvMatA, this, &ControllerTrackingAlgorithm::receiveCvMatFromTrackingAlgorithm);
    QObject::connect(trackingAlg, &BioTrackerTrackingAlgorithm::emitTrackingDone, this, &ControllerTrackingAlgorithm::receiveTrackingDone);

}

void ControllerTrackingAlgorithm::receiveCvMatFromTrackingAlgorithm(std::shared_ptr<cv::Mat> mat, QString name)
{
    Q_EMIT emitCvMat(mat, name);
}

void ControllerTrackingAlgorithm::receiveTrackingDone()
{
    Q_EMIT emitTrackingDone();
}
