/****************************************************************************
 **
 ** This file is part of the BioTracker Framework
 ** by Andreas Jörg
 **
 ****************************************************************************/

#ifndef PSTATESTEPFORW_H
#define PSTATESTEPFORW_H

#include "IStates/IPlayerState.h"

/**
 * This State is active if the user clicks the Forward button in the GUI. The
 * next state will be STATE_WAIT.
 */
class PStateStepForw : public IPlayerState
{
public:
    PStateStepForw(MediaPlayerStateMachine*                       player,
                   std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

    // IPlayerState interface
public Q_SLOTS:
    void operate() override;
};

#endif // PSTATESTEPFORW_H
