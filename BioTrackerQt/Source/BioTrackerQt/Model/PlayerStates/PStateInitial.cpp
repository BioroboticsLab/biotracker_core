#include "PStateInitial.h"
#include "Model/BioTracker3Player.h"

PStateInitial::PStateInitial(BioTracker3Player *player,
                             std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, imageStream) {
    operate();
}

void PStateInitial::operate() {

    m_Play = false;
    m_Forw = false;
    m_Back = false;
    m_Stop = false;
    m_Paus = false;

    m_Mat = cv::Mat(320, 240, CV_8UC3, cv::Scalar(0,0,0));
    m_FrameNumber = 0;

  //  Q_EMIT emitStateDone();

    m_Player->setNextState(IPlayerState::STATE_WAIT);
}
