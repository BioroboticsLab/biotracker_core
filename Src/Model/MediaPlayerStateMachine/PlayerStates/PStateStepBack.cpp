#include "PStateStepBack.h"
#include "Model/MediaPlayerStateMachine/MediaPlayerStateMachine.h"

PStateStepBack::PStateStepBack(MediaPlayerStateMachine* player,
                               std::shared_ptr<BioTracker::Core::ImageStream> imageStream) :
    IPlayerState(player, imageStream)  {

    m_StateParameters.m_Back = false;
    m_StateParameters.m_Forw = false;
    m_StateParameters.m_Paus = false;
    m_StateParameters.m_Play = false;
    m_StateParameters.m_Stop = false;
	m_StateParameters.m_RecI = false;
	m_StateParameters.m_RecO = false;

    m_FrameNumber = 0;
    m_prevInBatch = false;
}

void PStateStepBack::setPrevInBatch(bool prev) {
    m_prevInBatch = prev;
}

void PStateStepBack::operate() {

    m_StateParameters.m_Play = true;
    m_StateParameters.m_Forw = true;
    m_StateParameters.m_Stop = true;
    m_StateParameters.m_Paus = false;

    IPlayerState::PLAYER_STATES nextState = IPlayerState::STATE_INITIAL;

    // skip to next medium in batch if existing
    if (m_prevInBatch && m_ImageStream->hasPrevInBatch()) {
        m_StateParameters.m_Play = true;
        m_StateParameters.m_Forw = false;
        m_StateParameters.m_Back = false;
        m_StateParameters.m_Stop = true;
        m_StateParameters.m_Paus = true;

        Q_EMIT emitNextMediaInBatch(m_ImageStream->currentFilename()); 
        m_ImageStream->stepToPrevInBatch();
        m_ImageStream->nextFrame();
        m_Mat = m_ImageStream->currentFrame();
        m_FrameNumber = m_ImageStream->currentFrameNumber();    
        nextState = IPlayerState::STATE_PLAY;
        Q_EMIT emitNextMediaInBatchLoaded(m_ImageStream->currentFilename(), m_ImageStream->currentBatchIndex());

        m_prevInBatch = false;
    }
    else{
        if (m_ImageStream->previousFrame()) {
                    m_Mat = m_ImageStream->currentFrame();
                    m_FrameNumber = m_ImageStream->currentFrameNumber();
        }

        bool stateBack = false;
        if (m_FrameNumber <= 0) {
            stateBack = false;
        } else {
            stateBack = true;
        }

        m_StateParameters.m_Back = stateBack;
        nextState = IPlayerState::STATE_WAIT;
    }

    m_Player->setNextState(nextState);
}
