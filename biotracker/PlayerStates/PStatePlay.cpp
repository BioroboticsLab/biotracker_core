#include "PStatePlay.h"
#include "BioTracker3Player.h"

PStatePlay::PStatePlay(BioTracker3Player *player, IModel *textureObject,
                       std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, textureObject, imageStream) {

}

void PStatePlay::operate() {
    m_ImageStream->nextFrame();

    dynamic_cast<BioTracker::Core::BioTracker3TextureObject *>(m_TextureObjectModel)->set(m_ImageStream->currentFrame());

    Q_EMIT m_Player->notifyView();

    Q_EMIT emitNextState(IPlayerState::STATE_PLAY);
}

bool PStatePlay::stateOfPlay() {
    return true;
}

bool PStatePlay::stateOfRew() {
    return false;
}

bool PStatePlay::stateOfStepForward() {
    return false;
}

bool PStatePlay::stateOfStop() {
    return true;
}
