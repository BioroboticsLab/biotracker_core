/****************************************************************************
 **
 ** This file is part of the BioTracker Framework
 ** by Andreas Jörg
 **
 ****************************************************************************/

#ifndef PSTATEPAUSE_H
#define PSTATEPAUSE_H

#include "IStates/IPlayerState.h"

/**
 * This state is active when the user clicks on the Pause button in the GUI.
 * The next state will be STATE_WAIT.
 */
class PStatePause : public IPlayerState
{
public:
    PStatePause(MediaPlayerStateMachine*                       player,
                std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

    // IPlayerState interface
public Q_SLOTS:
    void operate() override;
};

#endif // PSTATEPAUSE_H
