#include "StateVideoStepForward.h"
#include "BioTracker3Player.h"

StateVideoStepForward::StateVideoStepForward(BioTracker3Player *player, IModel *textureObject,
        std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, textureObject, imageStream) {

}

void StateVideoStepForward::doIt() {
    m_ImageStream->nextFrame();

    dynamic_cast<BioTracker::Core::BioTracker3TextureObject *>(m_TextureObjectModel)->set(m_ImageStream->currentFrame());


    Q_EMIT m_Player->notifyView();
}

bool StateVideoStepForward::stateOfPlay() {
    return true;
}

bool StateVideoStepForward::stateOfRew() {
    return true;
}

bool StateVideoStepForward::stateOfStepForward() {
    return true;
}

bool StateVideoStepForward::stateOfStop() {
    return true;
}
