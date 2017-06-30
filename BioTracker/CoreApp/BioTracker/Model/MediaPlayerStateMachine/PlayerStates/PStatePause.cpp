#include "PStatePause.h"
#include "Model/MediaPlayerStateMachine/MediaPlayerStateMachine.h"

PStatePause::PStatePause(MediaPlayerStateMachine* player,
                         std::shared_ptr<BioTracker::Core::ImageStream> imageStream) :
    IPlayerState(player, imageStream) {

    m_StateParameters.m_Back = false;
    m_StateParameters.m_Forw = false;
    m_StateParameters.m_Paus = false;
    m_StateParameters.m_Play = false;
    m_StateParameters.m_Stop = false;
	m_StateParameters.m_RecI = false;
	m_StateParameters.m_RecO = false;

    m_FrameNumber = 0;

}

void PStatePause::operate() {

    m_StateParameters.m_Play = true;
    m_StateParameters.m_Forw = true;
    m_StateParameters.m_Back = true;
    m_StateParameters.m_Stop = true;
    m_StateParameters.m_Paus = false;

    m_Mat = m_ImageStream->currentFrame();
    m_FrameNumber = m_ImageStream->currentFrameNumber();

    m_Player->setNextState(IPlayerState::STATE_WAIT);
}
