#include "PStateInitialStream.h"
#include "Model/MediaPlayerStateMachine/MediaPlayerStateMachine.h"

PStateInitialStream::PStateInitialStream(
    MediaPlayerStateMachine*                       player,
    std::shared_ptr<BioTracker::Core::ImageStream> imageStream)
: IPlayerState(player, imageStream)
{

    m_StateParameters.m_Back = false;
    m_StateParameters.m_Forw = false;
    m_StateParameters.m_Paus = false;
    m_StateParameters.m_Play = false;
    m_StateParameters.m_Stop = false;

    m_FrameNumber = 0;
}

void PStateInitialStream::operate()
{

    m_StateParameters.m_Play = true;
    m_StateParameters.m_Forw = true;
    m_StateParameters.m_Back = false;
    m_StateParameters.m_Stop = false;
    m_StateParameters.m_Paus = false;

    bool xval = false;
    xval      = m_ImageStream->setFrameNumber(0);

    if (xval) {
        m_Mat         = m_ImageStream->currentFrame();
        m_FrameNumber = m_ImageStream->currentFrameNumber();
    }

    m_Player->setNextState(IPlayerState::STATE_WAIT);
}
