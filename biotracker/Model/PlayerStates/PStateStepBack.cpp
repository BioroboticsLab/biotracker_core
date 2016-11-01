#include "PStateStepBack.h"
#include "Model/BioTracker3Player.h"

PStateStepBack::PStateStepBack(BioTracker3Player *player, IModel *textureObject,
                               std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, textureObject, imageStream)  {

}

void PStateStepBack::operate() {
    if (m_ImageStream->previousFrame()) {
        dynamic_cast<BioTracker::Core::BioTracker3TextureObject *>(m_TextureObjectModel)->set(m_ImageStream->currentFrame());
    }

    bool stateBack = false;
    if (m_ImageStream->currentFrameNumber() <= 0) {
        stateBack = false;
    } else {
        stateBack = true;
    }

    Q_EMIT emitStateOfPlay(true);
    Q_EMIT emitStateOfStepForward(true);
    Q_EMIT emitStateOfStepBackward(stateBack);
    Q_EMIT emitStateOfStop(true);
    Q_EMIT emitStateOfPause(false);

    Q_EMIT emitNextState(IPlayerState::STATE_WAIT);

    Q_EMIT emitOperationDone();
}
