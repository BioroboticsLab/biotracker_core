#include "ControllerTrackingAlgorithm.h"

ControllerTrackingAlgorithm::ControllerTrackingAlgorithm()
{

}

void ControllerTrackingAlgorithm::connectViewToMainWindow(IController *controller)
{
    IView *view = controller->getView();
  //  static_cast<BioTracker3MainWindow *>(view)->addVideoView(m_View);
}

void ControllerTrackingAlgorithm::connectToOtherController(IController *controller)
{
//    ControllerPlayer *ctr = dynamic_cast<ControllerPlayer *>(controller);
//    BioTracker3Player *player = dynamic_cast<BioTracker3Player *>(ctr->getModel());

//    QObject::connect(player, &BioTracker3Player::emitCurrentFrame, this, &ControllerTextureObject::receiveCvMat);

}

void ControllerTrackingAlgorithm::createModel()
{

}

void ControllerTrackingAlgorithm::createView()
{

}

void ControllerTrackingAlgorithm::connectModelController()
{

}

void ControllerTrackingAlgorithm::connectModelView()
{

}
