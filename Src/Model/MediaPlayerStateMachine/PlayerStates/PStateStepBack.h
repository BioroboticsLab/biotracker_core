/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/

#ifndef PSTATESTEPBACK_H
#define PSTATESTEPBACK_H

#include "IStates/IPlayerState.h"

/**
 * This state will active if the user clicks on the Back button in the GUI. The following state will be STATE_WAIT.
 */
class PStateStepBack : public IPlayerState {
  public:
    PStateStepBack(MediaPlayerStateMachine* player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

  /**
   * This function sets the previous medium flag.
   */
  void setPrevInBatch(bool next);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;

  private:
    bool m_prevInBatch;
};

#endif // PSTATESTEPBACK_H
