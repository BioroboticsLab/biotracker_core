#include "PStatePlay.h"
#include "Model/BioTracker3Player.h"
#include "QTimer"

PStatePlay::PStatePlay(BioTracker3Player *player,
                       std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, imageStream) {

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
        nextState = IPlayerState::STATE_PLAY;
    }
    else {
        nextState = IPlayerState::STATE_INITIAL_STREAM;
    }


    m_Player->setNextState(nextState);

//    Q_EMIT emitStateDone();
}
