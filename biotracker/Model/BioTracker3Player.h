#ifndef BIOTRACKER3PLAYER_H
#define BIOTRACKER3PLAYER_H

#include <QObject>
#include "Interfaces/imodel.h"

#include "BioTracker3ImageStream.h"
#include <memory>
#include "QString"
#include "QMap"
#include "QThread"

#include "../BioTrackerGui/biotracker/View/BioTracker3VideoControllWidget.h"
#include "../BioTrackerGui/biotracker/View/BioTracker3VideoView.h"

#include "Interfaces/IPlayerState.h"


class BioTracker3Player : public IModel {
    Q_OBJECT
  public:
    explicit BioTracker3Player(BioTracker::Core::BioTracker3TextureObject *textureObject);

    void operate();

    // Interface for the States
    bool getStateOfStepForward();
    bool getStateOfPlay();
    bool getStateOfStepBack();
    bool getStateOfStop();
    bool getStateOfPause();

    IModel *getStateModel();


  public Q_SLOTS:

    void setStateOfStepForward(bool xState);
    void setStateOfStepBackward(bool xState);
    void setStateOfPlay(bool xState);
    void setStateOfStop(bool xState);
    void setStateOfPause(bool xState);


    void loadVideo(QString str);
    void nextFrame();
    void prevFrame();
    void play();
    void stop();
    void pause();

    void setNextState(IPlayerState::PLAYER_STATES state);
    void setCurrentState(IPlayerState::PLAYER_STATES state);

    void handelStateDone();

  Q_SIGNALS:
    void emitChangeImageStream(std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> imageStream);
    void emitStateChangeDone();

    void emitTrackThisImage(cv::Mat image);

  private:
    IPlayerState *m_CurrentPlayerState;


    QThread m_StateThread;
    QMap<IPlayerState::PLAYER_STATES, IPlayerState *> m_States;

    IModel *m_TextureObjectModel;
    std::shared_ptr<BioTracker::Core::BioTracker3ImageStream> m_ImageStream;

    bool m_StateOfStepForward;
    bool m_StateOfStepBack;
    bool m_StateOfPlay;
    bool m_StateOfStop;
    bool m_StateOfPause;
};

#endif // BIOTRACKER3PLAYER_H
