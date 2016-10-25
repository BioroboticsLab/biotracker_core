#include "IPlayerState.h"

#include "Model/BioTracker3Player.h"

#include "opencv2/core/core.hpp"

IPlayerState::IPlayerState(BioTracker3Player *player, IModel *textureObjectModel,
                           std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    m_Player(player),
    m_TextureObjectModel(dynamic_cast<BioTracker::Core::BioTracker3TextureObject *>(textureObjectModel)),
    m_ImageStream(imageStream) {

}

void IPlayerState::operate() {

}

void IPlayerState::changeImageStream(std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) {
    m_ImageStream.reset();
    m_ImageStream = imageStream;
}
