#include "IPlayerState.h"

#include "Model/MediaPlayerStateMachine/MediaPlayerStateMachine.h"

#include "opencv2/core/core.hpp"

IPlayerState::IPlayerState(MediaPlayerStateMachine* player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream) :
    m_Player(player),
    m_ImageStream(imageStream) {

}

std::shared_ptr<cv::Mat> IPlayerState::getCurrentFrame() {
    return m_Mat;
}

size_t IPlayerState::getCurrentFrameNumber() {
    return m_FrameNumber;
}

void IPlayerState::changeImageStream(std::shared_ptr<BioTracker::Core::ImageStream> imageStream) {
    m_ImageStream.reset();
    m_ImageStream = imageStream;
}

stateParameters IPlayerState::getStateParameters() {
    return m_StateParameters;
}

QString IPlayerState::getCurrentFileName() {
    return QString::fromStdString( m_ImageStream->currentFilename() );
}
