#include "PStateStepForw.h"
#include "Model/BioTracker3Player.h"

PStateStepForw::PStateStepForw(BioTracker3Player *player,
                               std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, imageStream) {

}

void PStateStepForw::operate() {

    m_Play = true;
    m_Back = true;
    m_Stop = true;
    m_Paus = false;


    if (m_ImageStream->nextFrame()) {
        m_Mat = m_ImageStream->currentFrame();
        m_FrameNumber = m_ImageStream->currentFrameNumber();
    }

    bool stateFw = false;
    if (m_ImageStream->lastFrame()) {
        stateFw = false;
    } else {
        stateFw = true;
    }

    m_Forw = true;

//    Q_EMIT emitStateDone();

    m_Player->setNextState(IPlayerState::STATE_WAIT);
}
