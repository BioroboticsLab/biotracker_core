#include "PStateGoToFrame.h"
#include "Model/MediaPlayerStateMachine/MediaPlayerStateMachine.h"

PStateGoToFrame::PStateGoToFrame(
    MediaPlayerStateMachine*                       player,
    std::shared_ptr<BioTracker::Core::ImageStream> imageStream)
: IPlayerState(player, imageStream)
{
    m_StateParameters.m_Back = false;
    m_StateParameters.m_Forw = false;
    m_StateParameters.m_Paus = false;
    m_StateParameters.m_Play = false;
    m_StateParameters.m_Stop = false;
    m_StateParameters.m_RecI = false;
    m_StateParameters.m_RecO = false;

    m_FrameNumber = 0;

    m_GoToFrameNumber = 0;

    operate();
}

void PStateGoToFrame::setFrameNumber(int frame)
{
    m_GoToFrameNumber = frame;
}

void PStateGoToFrame::operate()
{
    m_StateParameters.m_Play = true;
    m_StateParameters.m_Stop = true;
    m_StateParameters.m_Paus = false;

    if (m_ImageStream->setFrameNumber(m_GoToFrameNumber)) {
        m_Mat         = m_ImageStream->currentFrame();
        m_FrameNumber = m_ImageStream->currentFrameNumber();
    }

    m_StateParameters.m_Forw = false;
    if (m_ImageStream->lastFrame()) {
        m_StateParameters.m_Forw = false;
    } else {
        m_StateParameters.m_Forw = true;
    }

    m_StateParameters.m_Forw = true;

    m_StateParameters.m_Back = false;
    if (m_FrameNumber <= 0) {
        m_StateParameters.m_Back = false;
    } else {
        m_StateParameters.m_Back = true;
    }

    m_Player->setNextState(IPlayerState::STATE_WAIT);
}
