#ifndef IPLAYERSTATE_H
#define IPLAYERSTATE_H

#include <QObject>
#include "QPointer"

#include "Interfaces/IModel/IModel.h"

#include "Model/ImageStream.h"
#include "Model/TextureObject.h"

struct stateParameters {
    bool m_Play;
    bool m_Forw;
    bool m_Back;
    bool m_Stop;
    bool m_Paus;
};

class MediaPlayerStateMachine;
class IPlayerState : public IModel {
    Q_OBJECT
  public:
    explicit IPlayerState(MediaPlayerStateMachine* player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream);
    enum PLAYER_STATES {STATE_INITIAL, STATE_INITIAL_STREAM, STATE_PLAY, STATE_STEP_FORW,
                        STATE_STEP_BACK, STATE_PAUSE, STATE_WAIT, STATE_GOTOFRAME
                       };
    Q_ENUM(PLAYER_STATES)


  public:
    void changeImageStream(std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

    virtual void operate() = 0;

    stateParameters getStateParameters();
    QString getCurrentFileName();
    std::shared_ptr<cv::Mat> getCurrentFrame();
    size_t getCurrentFrameNumber();

  protected:
    MediaPlayerStateMachine* m_Player;
    std::shared_ptr<BioTracker::Core::ImageStream> m_ImageStream;

    stateParameters m_StateParameters;

    std::shared_ptr<cv::Mat> m_Mat;
    size_t m_FrameNumber;

    QString m_currentFileName;
};

#endif // IPLAYERSTATE_H
