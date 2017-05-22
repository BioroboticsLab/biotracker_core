#ifndef IPLAYERSTATE_H
#define IPLAYERSTATE_H

#include <QObject>
#include "QPointer"

#include "Interfaces/IModel/IModel.h"

#include "Model/ImageStream.h"
#include "Model/TextureObject.h"

class MediaPlayer;
class IPlayerState : public IModel {
    Q_OBJECT
  public:
    explicit IPlayerState(MediaPlayer *player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream);
    enum PLAYER_STATES {STATE_INITIAL, STATE_INITIAL_STREAM, STATE_PLAY, STATE_STEP_FORW,
                        STATE_STEP_BACK, STATE_PAUSE, STATE_WAIT
                       };
    Q_ENUM(PLAYER_STATES)


  public:
    void changeImageStream(std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

    virtual void operate() = 0;

    bool getStateForPlay();
    bool getStateForForward();
    bool getStateForBackward();
    bool getStateForStop();
    bool getStateForPause();
    std::shared_ptr<cv::Mat> getCurrentFrame();
    size_t getCurrentFrameNumber();

Q_SIGNALS:
    void emitStateDone();

  protected:
    MediaPlayer *m_Player;
    std::shared_ptr<BioTracker::Core::ImageStream> m_ImageStream;

    bool m_Play;
    bool m_Forw;
    bool m_Back;
    bool m_Stop;
    bool m_Paus;

    std::shared_ptr<cv::Mat> m_Mat;
    size_t m_FrameNumber;
	std::fstream fs;


};

#endif // IPLAYERSTATE_H
