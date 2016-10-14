#include "PStateStepBack.h"
#include "BioTracker3Player.h"

PStateStepBack::PStateStepBack(BioTracker3Player *player, IModel *textureObject,
                               std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, textureObject, imageStream)  {

}

void PStateStepBack::operate() {
    if (m_ImageStream->previousFrame()) {
        dynamic_cast<BioTracker::Core::BioTracker3TextureObject *>(m_TextureObjectModel)->set(m_ImageStream->currentFrame());
    }


//    m_Player-> setStateOfPlay(true);
//    m_Player-> setStateOfStepForward(true);

    bool stateBack = false;
    if (m_ImageStream->currentFrameNumber() <= 0) {
        stateBack = false;
    } else {
        stateBack = true;
    }

    Q_EMIT stateOfPlay(true);
    Q_EMIT stateOfStepForward(true);
    Q_EMIT stateOfStepBackward(stateBack);
    Q_EMIT stateOfStop(false);

//    m_Player-> setStateOfStepBackward(stateBack);
//    m_Player-> setStateOfStop(false);

    //Q_EMIT m_Player->notifyView();

    Q_EMIT emitNextState(IPlayerState::STATE_PAUSE);

}
