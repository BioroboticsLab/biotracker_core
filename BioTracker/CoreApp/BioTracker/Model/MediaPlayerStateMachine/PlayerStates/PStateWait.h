/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/


#ifndef PSTATEWAIT_H
#define PSTATEWAIT_H

#include "IStates/IPlayerState.h"

/**
 * This state dose nothing. There is also no next state set by this state.
 */
class PStateWait : public IPlayerState {
  public:
    PStateWait(MediaPlayerStateMachine* player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;
};

#endif // PSTATEWAIT_H
