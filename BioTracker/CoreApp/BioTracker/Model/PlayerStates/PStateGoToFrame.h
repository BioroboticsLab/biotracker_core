#ifndef PSTATEGOTOFRAME_H
#define PSTATEGOTOFRAME_H

#include "IStates/IPlayerState.h"


class PStateGoToFrame : public IPlayerState
{
public:
    PStateGoToFrame(MediaPlayer *player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

    // IPlayerState interface
public Q_SLOTS:
  void operate() override;

public:
  void setFrameNumber(int frame);

private:
    int m_GoToFrameNumber;
};

#endif // PSTATEGOTOFRAME_H
