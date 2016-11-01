#include "PStateStepForw.h"
#include "Model/BioTracker3Player.h"

PStateStepForw::PStateStepForw(BioTracker3Player *player, IModel *textureObject,
                               std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, textureObject, imageStream) {

}

void PStateStepForw::operate() {

    if (m_ImageStream->nextFrame()) {
        dynamic_cast<BioTracker::Core::BioTracker3TextureObject *>(m_TextureObjectModel)->set(m_ImageStream->currentFrame());
    }

    bool stateFw = false;
    if (m_ImageStream->lastFrame()) {
        stateFw = false;
    } else {
        stateFw = true;
    }

    Q_EMIT emitStateOfPlay(true);
    Q_EMIT emitStateOfStepForward(stateFw);
    Q_EMIT emitStateOfStepBackward(true);
    Q_EMIT emitStateOfStop(true);
    Q_EMIT emitStateOfPause(false);

    Q_EMIT emitNextState(IPlayerState::STATE_WAIT);

    Q_EMIT m_Player->emitTrackThisImage(m_ImageStream->currentFrame());

    Q_EMIT emitOperationDone();
}
