#include "InitialVideoState.h"
#include "BioTracker3Player.h"

InitialVideoState::InitialVideoState(BioTracker3Player *player, IModel *textureObject,
                                     std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, textureObject, imageStream) {

}

void InitialVideoState::doIt() {
    cv::Mat mat = m_ImageStream->currentFrame();
    dynamic_cast<BioTracker::Core::BioTracker3TextureObject *>(m_TextureObjectModel)->set(mat);

    Q_EMIT m_Player->notifyView();
}

bool InitialVideoState::stateOfPlay() {
    return true;
}

bool InitialVideoState::stateOfRew() {
    return false;
}

bool InitialVideoState::stateOfStepForward() {
    return true;
}

bool InitialVideoState::stateOfStop() {
    return false;
}
