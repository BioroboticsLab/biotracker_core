#include "BioTrackerPlugin.h"

#include "Controller/ControllerTrackingAlgorithm.h"

BioTrackerPlugin::BioTrackerPlugin()
{
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(BioTrackerPlugin, BioTrackerPlugin)
#endif // QT_VERSION < 0x050000


void BioTrackerPlugin::createPlugin()
{

    m_TrackerController = new ControllerTrackingAlgorithm(this, 0, ENUMS::CONTROLLERTYPE::TRACKING);

    m_TrackerController->createComponents();
    m_TrackerController->connectComponents();

    connectInterfaces();

}

void BioTrackerPlugin::connectInterfaces()
{
    ControllerTrackingAlgorithm *ctrAlg = qobject_cast<ControllerTrackingAlgorithm *> (m_TrackerController);
    QObject::connect(ctrAlg, &ControllerTrackingAlgorithm::emitCvMat, this, &BioTrackerPlugin::receiveCvMatFromController);

    QObject::connect(ctrAlg, &ControllerTrackingAlgorithm::emitTrackingDone, this, &BioTrackerPlugin::receiveTrackingDone);
}

void BioTrackerPlugin::receiveCvMat(std::shared_ptr<cv::Mat> mat)
{
    qobject_cast<ControllerTrackingAlgorithm *> (m_TrackerController)->doTracking(mat);
}

void BioTrackerPlugin::receiveCvMatFromController(std::shared_ptr<cv::Mat> mat, QString name)
{
    Q_EMIT emitCvMat(mat, name);
}

void BioTrackerPlugin::receiveTrackingDone()
{
    Q_EMIT emitTrackingDone();
}
