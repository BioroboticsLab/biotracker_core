#include "PStateGoToFrame.h"
#include "Model/MediaPlayer.h"


PStateGoToFrame::PStateGoToFrame(MediaPlayer *player,
                                 std::shared_ptr<BioTracker::Core::ImageStream> imageStream) :
        IPlayerState(player, imageStream)
{
    m_GoToFrameNumber = 0;

    operate();

}

void PStateGoToFrame::setFrameNumber(int frame)
{
    m_GoToFrameNumber = frame;
}

void PStateGoToFrame::operate()
{
    m_Play = true;
    m_Stop = true;
    m_Paus = false;


    if (m_ImageStream->setFrameNumber(m_GoToFrameNumber)) {
        m_Mat = m_ImageStream->currentFrame();
        m_FrameNumber = m_ImageStream->currentFrameNumber();
    }

    bool stateFw = false;
    if (m_ImageStream->lastFrame()) {
        stateFw = false;
    } else {
        stateFw = true;
    }

    m_Forw = true;

    bool stateBack = false;
    if (m_FrameNumber <= 0) {
        stateBack = false;
    } else {
        stateBack = true;
    }

    m_Back = stateBack;

    m_Player->setNextState(IPlayerState::STATE_WAIT);

}
