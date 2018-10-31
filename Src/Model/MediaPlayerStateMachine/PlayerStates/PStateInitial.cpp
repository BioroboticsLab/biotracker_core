#include "PStateInitial.h"
#include "Model/MediaPlayerStateMachine/MediaPlayerStateMachine.h"

PStateInitial::PStateInitial(MediaPlayerStateMachine* player,
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

    operate();
}

void PStateInitial::operate() {

    m_StateParameters.m_Back = false;
    m_StateParameters.m_Forw = false;
    m_StateParameters.m_Paus = false;
    m_StateParameters.m_Play = false;
    m_StateParameters.m_Stop = false;

    m_FrameNumber = 0;

    m_Player->setNextState(IPlayerState::STATE_WAIT);
}
