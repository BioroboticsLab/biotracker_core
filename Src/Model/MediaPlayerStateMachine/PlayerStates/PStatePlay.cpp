#include "PStatePlay.h"
#include "Model/MediaPlayerStateMachine/MediaPlayerStateMachine.h"
#include "QTimer"

#include <thread>
using namespace std::chrono;

PStatePlay::PStatePlay(MediaPlayerStateMachine* player,
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
    } else if (m_ImageStream->hasNextInBatch()){
        Q_EMIT emitNextMediaInBatch(m_ImageStream->currentFilename()); 
        m_ImageStream->stepToNextInBatch();
        m_ImageStream->nextFrame();
        m_Mat = m_ImageStream->currentFrame();
        m_FrameNumber = m_ImageStream->currentFrameNumber();    
        nextState = IPlayerState::STATE_PLAY;
        Q_EMIT emitNextMediaInBatchLoaded(m_ImageStream->currentFilename());
    } else
    {
        nextState = IPlayerState::STATE_INITIAL_STREAM;
        qDebug() << "CORE:  Last medium ended";
    }

    //If fps is limited, wait the neccessary time
    end = std::chrono::system_clock::now();
    if (_targetFps > 0) {
        long long dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        long long targetFps = (1. / _targetFps) * 1000.;

        if (dt < targetFps) {
            std::this_thread::sleep_for(std::chrono::milliseconds(targetFps - dt));
        }
    }
    start = std::chrono::system_clock::now();


    m_Player->setNextState(nextState);

}
