#include "PStateWait.h"
#include "Model/MediaPlayerStateMachine/MediaPlayerStateMachine.h"


PStateWait::PStateWait(MediaPlayerStateMachine* player,
                       std::shared_ptr<BioTracker::Core::ImageStream> imageStream) :
    IPlayerState(player, imageStream) {

    m_StateParameters.m_Back = false;
    m_StateParameters.m_Forw = false;
    m_StateParameters.m_Paus = false;
    m_StateParameters.m_Play = false;
    m_StateParameters.m_Stop = false;

    m_FrameNumber = 0;

}

void PStateWait::operate() {

    // Just Wait
}
