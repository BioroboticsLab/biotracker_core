#include "PStateInitialStream.h"
#include "Model/MediaPlayer.h"

PStateInitialStream::PStateInitialStream(MediaPlayer *player,
        std::shared_ptr<BioTracker::Core::ImageStream> imageStream) :
    IPlayerState(player, imageStream) {

}

void PStateInitialStream::operate() {

    m_Play = true;
    m_Forw = true;
    m_Back = false;
    m_Stop = false;
    m_Paus = false;

    bool xval = false;
    xval = m_ImageStream->setFrameNumber(0);

    if (xval) {
        m_Mat = m_ImageStream->currentFrame();
        m_FrameNumber = m_ImageStream->currentFrameNumber();
    }

    //Q_EMIT emitStateDone();

    m_Player->setNextState(IPlayerState::STATE_WAIT);
}
