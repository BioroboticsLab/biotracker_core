/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/

#ifndef PSTATEINITIAL_H
#define PSTATEINITIAL_H

#include "IStates/IPlayerState.h"

/**
 * This State is the first state when running BioTracker. It sets all parameters to false and sets as next state STATE_WAIT
 */
class PStateInitial : public IPlayerState {
  public:
    PStateInitial(MediaPlayerStateMachine* player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;
};

#endif // PSTATEINITIAL_H
