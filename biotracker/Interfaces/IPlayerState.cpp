#include "IPlayerState.h"

#include "BioTracker3Player.h"

#include "opencv2/core/core.hpp"

IPlayerState::IPlayerState(BioTracker3Player *player, IModel *textureObjectModel,
                           std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    m_Player(player),
    m_TextureObjectModel(dynamic_cast<BioTracker::Core::BioTracker3TextureObject *>(textureObjectModel)),
    m_ImageStream(imageStream) {

}

void IPlayerState::doIt() {

}

void IPlayerState::changeImageStream(std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) {
    m_ImageStream.reset();
    m_ImageStream = imageStream;
}

bool IPlayerState::stateOfPlay() {
    return false;
}

bool IPlayerState::stateOfStepForward() {
    return false;
}

bool IPlayerState::stateOfRew() {
    return false;
}

bool IPlayerState::stateOfStop() {
    return false;
}

