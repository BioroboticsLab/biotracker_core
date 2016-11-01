#include "PStatePlay.h"
#include "Model/BioTracker3Player.h"

PStatePlay::PStatePlay(BioTracker3Player *player, IModel *textureObject,
                       std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, textureObject, imageStream) {

}

void PStatePlay::operate() {

    bool isLastFrame = m_ImageStream->lastFrame();
    IPlayerState::PLAYER_STATES nextState = IPlayerState::STATE_INITIAL;

    if (!isLastFrame) {
        m_ImageStream->nextFrame();
        dynamic_cast<BioTracker::Core::BioTracker3TextureObject *>(m_TextureObjectModel)->set(m_ImageStream->currentFrame());
        nextState = IPlayerState::STATE_PLAY;
    } else {
        nextState = IPlayerState::STATE_INITIAL_STREAM;
    }

    Q_EMIT emitStateOfPlay(true);
    Q_EMIT emitStateOfStepForward(false);
    Q_EMIT emitStateOfStepBackward(false);
    Q_EMIT emitStateOfStop(true);
    Q_EMIT emitStateOfPause(true);

    Q_EMIT emitNextState(nextState);

    Q_EMIT m_Player->emitTrackThisImage(m_ImageStream->currentFrame());

    Q_EMIT emitOperationDone();
}
