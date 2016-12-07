#include "PStatePause.h"
#include "Model/MediaPlayer.h"

PStatePause::PStatePause(MediaPlayer *player,
                         std::shared_ptr<BioTracker::Core::ImageStream> imageStream) :
    IPlayerState(player, imageStream) {

}

void PStatePause::operate() {

    m_Play = true;
    m_Forw = true;
    m_Back = true;
    m_Stop = true;
    m_Paus = false;

    m_Mat = m_ImageStream->currentFrame();
    m_FrameNumber = m_ImageStream->currentFrameNumber();

  //  Q_EMIT emitStateDone();

    m_Player->setNextState(IPlayerState::STATE_WAIT);
}
