#include "TrackerController.h"
#include "Model/TrackedComponents/TrackedObjectFactory.h"
#include "Model/TrackedComponents/TrackedObject.h"
#include "Model/BioTrackerTrackingAlgorithm.h"
#include "Controller/BioTrackerController.h"
#include "Model/BioTracker3Player.h"

#include "../BioTrackerGui/biotracker/View/TrackedImageView.h"
#include "Model/BioTracker3TextureObject.h"


TrackerController::TrackerController() :
    IController(0) {

    m_TrackedComponent = 0;
    m_TrackedComponentFactory = 0;
    m_TrackingAlgorithm = 0;
}

void TrackerController::createApplication()
{
    m_TrackedComponentFactory = new TrackedObjectFactory();
    m_TrackingAlgorithm = new BioTrackerTrackingAlgorithm(0, m_TrackedComponentFactory);

    m_TrackingAlgorithm->setTrackedComponent(m_TrackedComponentFactory->getNewTrackedObject());

    m_TrackedTexture = new BioTracker::Core::BioTracker3TextureObject();

    dynamic_cast<BioTrackerTrackingAlgorithm *>(m_TrackingAlgorithm)->setTextureModel(m_TrackedTexture);


    m_TrackedImageView = new TrackedImageView(0, this, m_TrackedTexture);

    dynamic_cast<QOpenGLWidget *>(m_TrackedImageView)->show();


    m_TrackedComponentFactory->moveToThread(&m_TrackerThread);
    m_TrackingAlgorithm->moveToThread(&m_TrackerThread);

    m_TrackerThread.start();
}

void TrackerController::addPlayerController(IController *controller)
{
    m_PlayerController = controller;
    BioTrackerController *bioCtrl = dynamic_cast<BioTrackerController *>(m_PlayerController);
    BioTracker3Player *player = dynamic_cast<BioTracker3Player *>(bioCtrl->getPlayer());
    qRegisterMetaType< cv::Mat >("cv::Mat");

    QObject::connect(player, &BioTracker3Player::emitTrackThisImage, m_TrackingAlgorithm, &ITrackingAlgorithm::doTracking);
}
