/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/


#ifndef IPLAYERSTATE_H
#define IPLAYERSTATE_H

#include <QObject>
#include "QPointer"

#include "Interfaces/IModel/IModel.h"

#include "Model/ImageStream.h"
#include "Model/TextureObject.h"

/**
 * The stateParameters struct represents the possible stats for the VideoControll buttons in the MainWindow widget.
 * If a value is true, the button will be available to the user. If a value is false, the button is deactivated in the MainWindow.
 */
struct stateParameters {
    bool m_Play;
    bool m_Forw;
    bool m_Back;
    bool m_Stop;
	bool m_Paus;
	bool m_RecI;
	bool m_RecO;
};

class MediaPlayerStateMachine;
/**
 * The IPlayerState class is the interface for all states of the MediaPlayerStateMachine.
 * This class is responsible for manipulating ImageStreams with its method operate().
 * If new states are required simply create them and add a name for the state to the
 * ENUMERATION PLAYER_STATES.
 *
 * Any State class is able to set its following state in the MediaPlayerStateMachine.
 *
 */
class IPlayerState : public IModel {
    Q_OBJECT
  public:
    explicit IPlayerState(MediaPlayerStateMachine* player, std::shared_ptr<BioTracker::Core::ImageStream> imageStream);
    enum PLAYER_STATES {STATE_INITIAL, STATE_INITIAL_STREAM, STATE_PLAY, STATE_STEP_FORW,
                        STATE_STEP_BACK, STATE_PAUSE, STATE_WAIT, STATE_GOTOFRAME
                       };
    Q_ENUM(PLAYER_STATES)


  public:
    /**
     * If the user changes the Image Source all states will be informed throught this method.
     */
    void changeImageStream(std::shared_ptr<BioTracker::Core::ImageStream> imageStream);

    /**
     * Must be implemented by any state. Implement here the ImageStream manipulation.
     */
    virtual void operate() = 0;

    /**
     * Returns the State Parameters for the ImageStream controlls in the VideoViewWidget.
     */
    stateParameters getStateParameters();
    /**
     * Returns the current filename of the ImageStream.
     */
    QString getCurrentFileName();
    /**
     * Returns the current cv::Mat of the ImageStream.
     */
    std::shared_ptr<cv::Mat> getCurrentFrame();
    /**
     * Returns the current frame number of the ImageStream.
     */
    size_t getCurrentFrameNumber();

	//TODO: The Media player does not hold a valid imagestream. Hence use the current state's stream to grab details
	std::shared_ptr<BioTracker::Core::ImageStream> m_ImageStream;

  protected:
    MediaPlayerStateMachine* m_Player;

    stateParameters m_StateParameters;

    std::shared_ptr<cv::Mat> m_Mat;
    size_t m_FrameNumber;
	std::fstream fs;

    QString m_currentFileName;
};

#endif // IPLAYERSTATE_H
