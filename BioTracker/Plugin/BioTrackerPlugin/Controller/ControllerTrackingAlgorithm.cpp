#include "ControllerTrackingAlgorithm.h"

#include "Model/TrackerParameter.h"
#include "View/TrackerParameterView.h"

ControllerTrackingAlgorithm::ControllerTrackingAlgorithm(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr)
{

}

void ControllerTrackingAlgorithm::connectControllerToController()
{

}

void ControllerTrackingAlgorithm::doTracking(std::shared_ptr<cv::Mat> mat, uint number)
{
    qobject_cast<BioTrackerTrackingAlgorithm *>(m_Model)->doTracking(mat, number);
}

IView *ControllerTrackingAlgorithm::getTrackingParameterWidget()
{
    return m_View;
}

void ControllerTrackingAlgorithm::createModel()
{
    m_TrackingParameter = new TrackerParameter(this);
    m_Model = new BioTrackerTrackingAlgorithm(m_TrackingParameter);
}

void ControllerTrackingAlgorithm::createView()
{
    m_View = new TrackerParameterView(0, this, m_TrackingParameter);
}

void ControllerTrackingAlgorithm::connectModelToController()
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
