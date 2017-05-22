#include "PStatePlay.h"
#include "Model/MediaPlayerStateMachine/MediaPlayerStateMachine.h"
#include "QTimer"

PStatePlay::PStatePlay(MediaPlayerStateMachine* player,
                       std::shared_ptr<BioTracker::Core::ImageStream> imageStream) :
    IPlayerState(player, imageStream) {

    m_StateParameters.m_Back = false;
    m_StateParameters.m_Forw = false;
    m_StateParameters.m_Paus = false;
    m_StateParameters.m_Play = false;
    m_StateParameters.m_Stop = false;

    m_FrameNumber = 0;

}

void PStatePlay::operate() {

    m_StateParameters.m_Play = true;
    m_StateParameters.m_Forw = false;
    m_StateParameters.m_Back = false;
    m_StateParameters.m_Stop = true;
    m_StateParameters.m_Paus = true;

    bool isLastFrame = m_ImageStream->lastFrame();
    IPlayerState::PLAYER_STATES nextState = IPlayerState::STATE_INITIAL;

    if (!isLastFrame) {
        m_ImageStream->nextFrame();
        m_Mat = m_ImageStream->currentFrame();
        m_FrameNumber = m_ImageStream->currentFrameNumber(); 
        nextState = IPlayerState::STATE_PLAY;
    } else {
        nextState = IPlayerState::STATE_INITIAL_STREAM;
    }


    m_Player->setNextState(nextState);
}
