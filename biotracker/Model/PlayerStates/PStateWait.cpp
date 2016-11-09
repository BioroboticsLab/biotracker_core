#include "PStateWait.h"
#include "Model/BioTracker3Player.h"


PStateWait::PStateWait(BioTracker3Player *player,
                       std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream) :
    IPlayerState(player, imageStream) {

}

void PStateWait::operate() {

 //   Q_EMIT emitStateDone();

    // Just Wait
}
