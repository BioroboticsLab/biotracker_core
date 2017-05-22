/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/

#ifndef PSTATEINITIALSTREAM_H
#define PSTATEINITIALSTREAM_H

#include "IStates/IPlayerState.h"

/**
 * This State is active when a video or camera device was selected by the user. It sets as following state STATE_WAIT.
 */
class PStateInitialStream : public IPlayerState {
  public:
    PStateInitialStream(MediaPlayerStateMachine* player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;
};

#endif // INITIALVIDEOSTATE_H
