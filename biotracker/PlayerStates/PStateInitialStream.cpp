#include "PStateInitialStream.h"
#include "BioTracker3Player.h"

PStateInitialStream::PStateInitialStream(BioTracker3Player *player, IModel *textureObject,
        std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, textureObject, imageStream) {

}

void PStateInitialStream::operate() {
    cv::Mat mat = m_ImageStream->currentFrame();
    dynamic_cast<BioTracker::Core::BioTracker3TextureObject *>(m_TextureObjectModel)->set(mat);

    Q_EMIT m_Player->notifyView();
}

bool PStateInitialStream::stateOfPlay() {
    return true;
}

bool PStateInitialStream::stateOfRew() {
    return false;
}

bool PStateInitialStream::stateOfStepForward() {
    return true;
}

bool PStateInitialStream::stateOfStop() {
    return false;
}
