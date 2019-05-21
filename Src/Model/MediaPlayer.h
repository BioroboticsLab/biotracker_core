/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/


#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include "Interfaces/IModel/IModel.h"
#include "QThread"
#include "Model/MediaPlayerStateMachine/MediaPlayerStateMachine.h"
#include "View/GraphicsView.h"

#include <ctime>
#include <chrono>
#include "util/types.h"
#include "util/VideoCoder.h"
#include "util/Config.h"

/**
 * The MediaPlayer class is an IModel class an part of the MediaPlayer component. This class creats a MediaPlayerStateMachine object and moves it to a QThread.
 * The MediaPlayer class controlls and represents the results of the MediaPlayerStateMachine. The communication between a MediaPlayer object and a MediaPlayerStateMachine object is done by
 * Qts SIGNALS and SLOTS. This is due to the fact that SIGNALS and SLTOS are thread safe. A disadvantage is that SIGNALS and SLOTS are slower then normal function calls.
 *
 * The constructor of this class is responsible for connecting to the MediaPlayerStateMachine.
 */
class MediaPlayer : public IModel {
    Q_OBJECT
  public:
    MediaPlayer(QObject* parent = 0);
    ~MediaPlayer();

  Q_SIGNALS:
    /**
    * Emit the path to a video stream. This signal will be received by the MediaPlayerStateMachine which runns in a separate Thread.
    */
    void loadVideoStream(std::vector<boost::filesystem::path> files);
    /**
    * Emit the path to pictures. This signal will be received by the MediaPlayerStateMachine which runns in a separate Thread.
    */
    void loadPictures(std::vector<boost::filesystem::path> files);
    /**
    * Emit the camera device number. This signal will be received by the MediaPlayerStateMachine which runns in a separate Thread.
    */
    void loadCameraDevice(CameraConfiguration conf);

    /**
    * Emit a frame number. This signal will be received by the MediaPlayerStateMachine which runns in a separate Thread.
    */
    void goToFrame(int frame);
    /**
    * Emit the next frame command. This signal will be received by the MediaPlayerStateMachine which runns in a separate Thread.
    */
    void nextFrameCommand();
    /**
    * Emit previous frame command. This signal will be received by the MediaPlayerStateMachine which runns in a separate Thread.
    */
    void prevFrameCommand();
    /**
    * Emit the play command. This signal will be received by the MediaPlayerStateMachine which runns in a separate Thread.
    */
    void playCommand();
    /**
    * Emit stop command. This signal will be received by the MediaPlayerStateMachine which runns in a separate Thread.
    */
    void stopCommand();
    /**
    * Emit the pause command. This signal will be received by the MediaPlayerStateMachine which runns in a separate Thread.
    */
    void pauseCommand();

    /**
     * This SIGNAL will be emmited if a state operation should be executed. This signal will be received by the MediaPlayerStateMachine which runns in a separate Thread.
     */
    void runPlayerOperation();

    /**
     * This SIGNAL will send a cv::Mat and a name to the MediaPlayer controller class. This controller will give the data to the TextureObject component.
     */
    void renderCurrentImage(std::shared_ptr<cv::Mat> mat, QString name);
    /**
     * This SIGNAL is only emmited if Tracking Is Active. The PluginLoader component will receive the cv::Mat and the current frame number.
     */
    void trackCurrentImage(std::shared_ptr<cv::Mat> mat, uint number);
	/**
	* This SIGNAL is only emmited if Tracking Is inactive. The core visualization controller will receive the framenumber and will try to visualize the tracking model.
	*/
	void signalVisualizeCurrentModel(uint frameNumber);

	void signalCurrentFrameNumberToPlugin(uint frameNumber);

	void toggleRecordImageStreamCommand();

	void fwdPlayerParameters(playerParameters* parameters);

    void emitNextMediaInBatch(const std::string path);
    void emitNextMediaInBatchLoaded(const std::string path);

  public:
    void setTrackingActive();
    void setTrackingDeactive();
    void setTargetFPS(double fps);

    bool getPlayState();
    bool getForwardState();
    bool getBackwardState();
    bool getStopState();
    bool getPauseState();
	bool getRecIState();
	bool getRecOState();

    bool getTrackingState();

	int toggleRecordGraphicsScenes(GraphicsView * gv);
	int toggleRecordImageStream();

    size_t getTotalNumberOfFrames();
    size_t getCurrentFrameNumber();
    double getFpsOfSourceFile();
    double getCurrentFPS();
    double getTargetFPS();
    QString getCurrentFileName();
    std::shared_ptr<cv::Mat> getCurrentFrame();

    QString takeScreenshot(GraphicsView *gv);

  public Q_SLOTS:
    /**
     * MediaPlayer will receive the current playerParameters from the MediaPlayerStateMachine.
     */
    void receivePlayerParameters(playerParameters* param);

    /**
     * If the MediaPlayerStateMachine is finished with executing the current state it will trigger this SLOT.
     */
    void receivePlayerOperationDone();

    /**
     * If a BioTracker Plugin is done with executing its tracking algorithm this SLOT will be triggerd. As soon as this SLOT is triggered, the MediaPlayerStateMachine will be
     * advised to execute the next state.
     */
    void receiveTrackingOperationDone();

    /**
    * 
    */
    void receiveTrackingPaused();

    /** 
     * Catches the pause state from the controller. 
     * This will automatically wrap all cases of "stop" semantics.
     * We do this although the info is basically passed implicitly via other commands, because it's easier
     */
    void rcvPauseState(bool state);

	void receiveChangeDisplayImage(QString str);

  private:
      //TODO Refactor members to _ instead of m_

	  /**
	  * helper function which opens a video. If video size has changed, a new video is opened. 
	  */
	int reopenVideoWriter();
	int _imagew;
	int _imageh;
    Config *_cfg;

    QPointer< QThread > m_PlayerThread;
    QPointer< MediaPlayerStateMachine > m_Player;

    //IPlayerState* m_CurrentPlayerState;
    //IPlayerState* m_NextPlayerState;

    QMap<IPlayerState::PLAYER_STATES, IPlayerState*> m_States;

    size_t m_TotalNumbFrames;
    size_t m_CurrentFrameNumber;
    double m_fpsOfSourceFile;
    double m_currentFPS;
    double m_targetFPS;
    QString m_CurrentFilename;
    std::shared_ptr<cv::Mat> m_CurrentFrame;

    bool m_Play;
    bool m_Forw;
    bool m_Back;
    bool m_Stop;
    bool m_Paus;
	bool m_RecI;
	bool m_RecO;


	bool m_recd;
	bool m_recordScaled;
	bool m_trackingDone;
    bool _paused = true;

	bool m_useCuda;
	GraphicsView *m_gv;
	QImage m_image;
	QPainter m_painter;
	std::shared_ptr<cv::VideoWriter> m_videoWriter;
	std::shared_ptr<VideoCoder> m_videoc;

    bool m_TrackingIsActive;
    QString m_NameOfCvMat = "Original";


    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point end;
};

#endif // MEDIAPLAYER_H
