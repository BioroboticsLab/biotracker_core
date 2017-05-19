/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/


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

/**
 * The MediaPlayerStateMachine class is an IModel class and is responsible for the executing and setting Player Stats. The instance of this class runns in a separate Thread.
 */
class MediaPlayerStateMachine : public IModel {
    Q_OBJECT
  public:
    explicit MediaPlayerStateMachine(QObject* parent = 0);

    /**
     * This method can be called by the MediaPlayer class and by any State which is part of the MediaPlayerStateMachine.
     */
    void setNextState(IPlayerState::PLAYER_STATES state);

  public Q_SLOTS:
    /**
     * This SLOT is called by the MediaPlayer class. If this slot is triggered the next state will be executed.
     */
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
    /**
     * After each state execution this SIGNAL is emmited and received by the MediaPlayer class. The parameter playerParameters contains all information that was changed during the execution of the current state.
     */
    void emitPlayerParameters(playerParameters* parameters);
    /**
     * When the state operation got finished, this SIGNAL is emmited and received by the MediaPlayer class.
     */
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
