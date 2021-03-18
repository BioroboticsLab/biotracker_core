#include "PStateStepForw.h"
#include "Model/MediaPlayerStateMachine/MediaPlayerStateMachine.h"

PStateStepForw::PStateStepForw(MediaPlayerStateMachine* player,
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
    m_nextInBatch = false;
}

void PStateStepForw::setNextInBatch(bool next) {
    m_nextInBatch = next;
}

void PStateStepForw::operate() {

    m_StateParameters.m_Play = true;
    m_StateParameters.m_Back = true;
    m_StateParameters.m_Stop = true;
    m_StateParameters.m_Paus = false;

    IPlayerState::PLAYER_STATES nextState = IPlayerState::STATE_INITIAL;

    // skip to next medium in batch if existing
    if (m_nextInBatch && m_ImageStream->hasNextInBatch()) {
        m_StateParameters.m_Play = true;
        m_StateParameters.m_Forw = false;
        m_StateParameters.m_Back = false;
        m_StateParameters.m_Stop = true;
        m_StateParameters.m_Paus = true;

        Q_EMIT emitNextMediaInBatch(m_ImageStream->currentFilename()); 
        m_ImageStream->stepToNextInBatch();
        m_ImageStream->nextFrame();
        m_Mat = m_ImageStream->currentFrame();
        m_FrameNumber = m_ImageStream->currentFrameNumber();    
        nextState = IPlayerState::STATE_PLAY;
        Q_EMIT emitNextMediaInBatchLoaded(m_ImageStream->currentFilename(), m_ImageStream->currentBatchIndex());

        m_nextInBatch = false;
    }
    // else just skip to next frame 
    else{
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

        m_StateParameters.m_Forw = stateFw;
        nextState = IPlayerState::STATE_WAIT;
    }

    m_Player->setNextState(nextState);
}
