#include "PStateWait.h"
#include "Model/MediaPlayer.h"


PStateWait::PStateWait(MediaPlayer *player,
                       std::shared_ptr<BioTracker::Core::ImageStream> imageStream) :
    IPlayerState(player, imageStream) {

}

void PStateWait::operate() {

 //   Q_EMIT emitStateDone();

    // Just Wait
}
