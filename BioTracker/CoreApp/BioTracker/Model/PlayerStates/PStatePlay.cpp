#include "PStatePlay.h"
#include "Model/MediaPlayer.h"
#include "QTimer"

#include <fstream>
#include <chrono>

PStatePlay::PStatePlay(MediaPlayer *player,
                       std::shared_ptr<BioTracker::Core::ImageStream> imageStream) :
    IPlayerState(player, imageStream) {
	fs.open("AcqBench.txt", std::fstream::in | std::fstream::out | std::fstream::app);
}

void PStatePlay::operate() {

//    QTimer *timer = new QTimer(this);

//    timer->start(1000);

//    while(timer->isActive());


    m_Play = true;
    m_Forw = false;
    m_Back = false;
    m_Stop = true;
    m_Paus = true;

    bool isLastFrame = m_ImageStream->lastFrame();
    IPlayerState::PLAYER_STATES nextState = IPlayerState::STATE_INITIAL;

    if (!isLastFrame) {
        m_ImageStream->nextFrame();
        m_Mat = m_ImageStream->currentFrame();
        m_FrameNumber = m_ImageStream->currentFrameNumber(); 
		auto now = std::chrono::system_clock::now();
		auto now_ns = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
		auto epoch = now_ns.time_since_epoch();
		auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
		ulong duration = value.count();
		fs << m_FrameNumber << "," << duration << std::endl;
        nextState = IPlayerState::STATE_PLAY;
    }
    else {
        nextState = IPlayerState::STATE_INITIAL_STREAM;
    }


    m_Player->setNextState(nextState);

//    Q_EMIT emitStateDone();
}
