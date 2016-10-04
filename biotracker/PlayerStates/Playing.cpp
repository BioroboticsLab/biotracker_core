#include "Playing.h"
#include "BioTracker3Player.h"

Playing::Playing(BioTracker3Player *player, IModel *textureObject,
                 std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, textureObject, imageStream) {

}

void Playing::doIt() {
    m_ImageStream->nextFrame();

    dynamic_cast<BioTracker::Core::BioTracker3TextureObject *>(m_TextureObjectModel)->set(m_ImageStream->currentFrame());

    Q_EMIT m_Player->notifyView();

    Q_EMIT emitNextState(IPlayerState::STATE_PLAY);
}

bool Playing::stateOfPlay() {
    return true;
}

bool Playing::stateOfRew() {
    return false;
}

bool Playing::stateOfStepForward() {
    return false;
}

bool Playing::stateOfStop() {
    return true;
}
