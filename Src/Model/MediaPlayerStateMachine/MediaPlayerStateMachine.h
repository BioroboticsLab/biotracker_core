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

#include "View/CameraDevice.h"
#include "util/Config.h"

/**
 * The MediaPlayerStateMachine class is an IModel class and is responsible for
 * the executing and setting Player Stats. The instance of this class runns in
 * a separate Thread.
 */
class MediaPlayerStateMachine : public IModel
{
    Q_OBJECT
public:
    explicit MediaPlayerStateMachine(QObject* parent = 0);

    /**
     * This method can be called by the MediaPlayer class and by any State
     * which is part of the MediaPlayerStateMachine.
     */
    void setNextState(IPlayerState::PLAYER_STATES state);

    IPlayerState::PLAYER_STATES getState();

    void setConfig(Config* cfg)
    {
        _cfg = cfg;
    };

public Q_SLOTS:
    /**
     * This SLOT is called by the MediaPlayer class. If this slot is triggered
     * the next state will be executed.
     */
    void receiveRunPlayerOperation();

    void receiveLoadVideoCommand(std::vector<boost::filesystem::path> files);
    void receiveLoadPictures(std::vector<boost::filesystem::path> files);
    void receiveLoadCameraDevice(CameraConfiguration conf);

    void receivePrevFrameCommand();
    void receiveNextFramCommand();
    void receivePauseCommand();
    void receiveStopCommand();
    void receivePlayCommand();
    void receiveGoToFrame(int frame);
    void receiveTargetFps(double fps);

    void receivetoggleRecordImageStream();

Q_SIGNALS:
    /**
     * After each state execution this SIGNAL is emmited and received by the
     * MediaPlayer class. The parameter playerParameters contains all
     * information that was changed during the execution of the current state.
     */
    void emitPlayerParameters(
        std::shared_ptr<const playerParameters> parameters);

    /**
     * When the state operation got finished, this SIGNAL is emmited and
     * received by the MediaPlayer class.
     */
    void emitPlayerOperationDone();

    void emitNextMediaInBatch(const std::string path);
    void emitNextMediaInBatchLoaded(const std::string path);

private:
    void updatePlayerParameter();
    void emitSignals();

private:
    IPlayerState*                                    m_CurrentPlayerState;
    IPlayerState*                                    m_NextPlayerState;
    QMap<IPlayerState::PLAYER_STATES, IPlayerState*> m_States;
    std::shared_ptr<BioTracker::Core::ImageStream>   m_ImageStream;

    playerParameters                               m_PlayerParameters;
    std::shared_ptr<BioTracker::Core::ImageStream> m_stream;
    Config*                                        _cfg;
};

#endif // BIOTRACKER3PLAYER_H
