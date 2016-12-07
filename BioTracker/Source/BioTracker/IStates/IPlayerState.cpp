#include "IPlayerState.h"

#include "Model/MediaPlayer.h"

#include "opencv2/core/core.hpp"

IPlayerState::IPlayerState(MediaPlayer *player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream) :
    m_Player(player),
    m_ImageStream(imageStream) {

}

bool IPlayerState::getStateForPlay()
{
    return m_Play;
}

bool IPlayerState::getStateForForward()
{
    return m_Forw;
}

bool IPlayerState::getStateForBackward()
{
    return m_Back;
}

bool IPlayerState::getStateForStop()
{
    return m_Stop;
}

bool IPlayerState::getStateForPause()
{
    return m_Paus;
}

std::shared_ptr<cv::Mat> IPlayerState::getCurrentFrame()
{
    return m_Mat;
}

size_t IPlayerState::getCurrentFrameNumber()
{
    return m_FrameNumber;
}

void IPlayerState::changeImageStream(std::shared_ptr<BioTracker::Core::ImageStream> imageStream) {
    m_ImageStream.reset();
    m_ImageStream = imageStream;
}
