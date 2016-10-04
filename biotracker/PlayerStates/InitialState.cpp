#include "InitialState.h"

InitialState::InitialState(BioTracker3Player *player, IModel *textureObject,
                           std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, textureObject, imageStream) {
    doIt();
}

void InitialState::doIt() {
    Q_EMIT notifyView();
}

bool InitialState::stateOfPlay() {
    return false;
}

bool InitialState::stateOfRew() {
    return false;
}

bool InitialState::stateOfStepForward() {
    return false;
}

bool InitialState::stateOfStop() {
    return false;
}
