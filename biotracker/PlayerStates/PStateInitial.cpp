#include "PStateInitial.h"

PStateInitial::PStateInitial(BioTracker3Player *player, IModel *textureObject,
                             std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, textureObject, imageStream) {
    operate();
}

void PStateInitial::operate() {
    Q_EMIT notifyView();
}

bool PStateInitial::stateOfPlay() {
    return false;
}

bool PStateInitial::stateOfRew() {
    return false;
}

bool PStateInitial::stateOfStepForward() {
    return false;
}

bool PStateInitial::stateOfStop() {
    return false;
}
