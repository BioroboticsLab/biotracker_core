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
 * This State is active if the user clicks the Forward button in the GUI. The next state will be STATE_WAIT.
 */
class PStateStepForw : public IPlayerState {
  public:
    PStateStepForw(MediaPlayerStateMachine* player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

    /**
     * This function sets the next medium flag.
     */
    void setNextInBatch(bool next);
    
    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;
  private:
    bool m_nextInBatch;
};

#endif // PSTATESTEPFORW_H
