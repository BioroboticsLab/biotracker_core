#ifndef IPLAYERSTATE_H
#define IPLAYERSTATE_H

#include <QObject>

#include "Interfaces/imodel.h"

#include "Model/BioTracker3ImageStream.h"
#include "Model/BioTracker3TextureObject.h"

class BioTracker3Player;
class IPlayerState : public IModel {
    Q_OBJECT
  public:
    explicit IPlayerState(BioTracker3Player *player, std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);
    enum PLAYER_STATES {STATE_INITIAL, STATE_INITIAL_STREAM, STATE_PLAY, STATE_STEP_FORW,
                        STATE_STEP_BACK, STATE_PAUSE, STATE_WAIT
                       };
    Q_ENUM(PLAYER_STATES)


  public:
    void changeImageStream(std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);

    virtual void operate() = 0;

    bool getStateForPlay();
    bool getStateForForward();
    bool getStateForBackward();
    bool getStateForStop();
    bool getStateForPause();
    cv::Mat getCurrentFrame();
    size_t getCurrentFrameNumber();

Q_SIGNALS:
    void emitStateDone();

  protected:
    BioTracker3Player *m_Player;
    std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> m_ImageStream;

    bool m_Play;
    bool m_Forw;
    bool m_Back;
    bool m_Stop;
    bool m_Paus;

    cv::Mat m_Mat;
    size_t m_FrameNumber;


};

#endif // IPLAYERSTATE_H
