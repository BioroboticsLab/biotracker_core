#ifndef BIOTRACKER3PLAYER_H
#define BIOTRACKER3PLAYER_H

#include <QObject>
#include "Interfaces/IModel/IModel.h"
#include "Model/ImageStream.h"
#include <memory>
#include "QString"
#include "QMap"
#include "QThread"
#include "opencv2/core/core.hpp"

#include "View/VideoControllWidget.h"
#include "View/GLVideoView.h"

#include "IStates/IPlayerState.h"
#include "QSharedPointer"

#include "PlayerParameters.h"


class MediaPlayerStateMachine : public IModel {
    Q_OBJECT
  public:
    explicit MediaPlayerStateMachine(QObject* parent = 0);

    void setNextState(IPlayerState::PLAYER_STATES state);

  public Q_SLOTS:
    void receiveRunPlayerOperation();

    void receiveLoadVideoCommand(QString fileDir);
    void receiveLoadPictures(std::vector<boost::filesystem::path> files);
    void receiveLoadCameraDevice(int i);

    void receivePrevFrameCommand();
    void receiveNextFramCommand();
    void receivePauseCommand();
    void receiveStopCommand();
    void receivePlayCommand();
    void receiveGoToFrame(int frame);

  Q_SIGNALS:
    void emitPlayerParameters(playerParameters* parameters);
    void emitPlayerOperationDone();

  private:
    void updatePlayerParameter();
    void emitSignals();


  private:
    IPlayerState* m_CurrentPlayerState;
    IPlayerState* m_NextPlayerState;
    QThread m_StateThread;
    QMap<IPlayerState::PLAYER_STATES, IPlayerState*> m_States;
    std::shared_ptr<BioTracker::Core::ImageStream> m_ImageStream;

    playerParameters* m_PlayerParameters;
};


#endif // BIOTRACKER3PLAYER_H
