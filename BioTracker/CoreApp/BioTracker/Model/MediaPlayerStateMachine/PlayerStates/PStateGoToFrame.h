/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/


#ifndef PSTATEGOTOFRAME_H
#define PSTATEGOTOFRAME_H

#include "IStates/IPlayerState.h"

/**
 * This State is responsible for jumping to a spezific frame number in the ImageStream. The following state will be STATE_WAIT
 */
class PStateGoToFrame : public IPlayerState {
  public:
    PStateGoToFrame(MediaPlayerStateMachine* player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

    // IPlayerState interface
  public Q_SLOTS:
    void operate() override;

  public:
    /**
     * This function sets the next frame number.
     */
    void setFrameNumber(int frame);

  private:
    int m_GoToFrameNumber;
};

#endif // PSTATEGOTOFRAME_H
