#include "PStateStepForw.h"
#include "BioTracker3Player.h"

PStateStepForw::PStateStepForw(BioTracker3Player *player, IModel *textureObject,
                               std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, textureObject, imageStream) {

}

void PStateStepForw::operate() {
    m_ImageStream->nextFrame();

    dynamic_cast<BioTracker::Core::BioTracker3TextureObject *>(m_TextureObjectModel)->set(m_ImageStream->currentFrame());


    Q_EMIT m_Player->notifyView();
}

bool PStateStepForw::stateOfPlay() {
    return true;
}

bool PStateStepForw::stateOfRew() {
    return true;
}

bool PStateStepForw::stateOfStepForward() {
    return true;
}

bool PStateStepForw::stateOfStop() {
    return true;
}
