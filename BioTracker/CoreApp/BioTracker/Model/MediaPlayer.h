#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include "Interfaces/IModel/IModel.h"
#include "QThread"
#include "Model/MediaPlayerStateMachine/MediaPlayerStateMachine.h"

class MediaPlayer : public IModel {
    Q_OBJECT
  public:
    MediaPlayer(QObject* parent = 0);
    ~MediaPlayer();

  Q_SIGNALS:
    /**
    * @brief loadVideoStream triggers the receiveVideoStream slot of the class MediaPlayerStateMachine
    * @param str is a QString with the Path to the video file
    */
    void loadVideoStream(QString str);
    void loadPictures(std::vector<boost::filesystem::path> files);
    void loadCameraDevice(int i);

    void goToFrame(int frame);
    void nextFrameCommand();
    void prevFrameCommand();
    void playCommand();
    void stopCommand();
    void pauseCommand();

    void runPlayerOperation();

    void renderCurrentImage(std::shared_ptr<cv::Mat> mat, QString name);
    void trackCurrentImage(std::shared_ptr<cv::Mat> mat, uint number);


  public:
    void setTrackingActive();
    void setTrackingDeactive();

    bool getPlayState();
    bool getForwardState();
    bool getBackwardState();
    bool getStopState();
    bool getPauseState();

    bool getTrackingState();

    size_t getTotalNumberOfFrames();
    size_t getCurrentFrameNumber();
    double getFpsOfSourceFile();
    double getCurrentFPS();
    QString getCurrentFileName();
    std::shared_ptr<cv::Mat> getCurrentFrame();


  public Q_SLOTS:
    void receivePlayerParameters(playerParameters* param);

    void receivePlayerOperationDone();
    void receiveTrackingOperationDone();


  private:
    QPointer< QThread > m_PlayerThread;
    QPointer< MediaPlayerStateMachine > m_Player;

    IPlayerState* m_CurrentPlayerState;
    IPlayerState* m_NextPlayerState;

    QMap<IPlayerState::PLAYER_STATES, IPlayerState*> m_States;

    size_t m_TotalNumbFrames;
    size_t m_CurrentFrameNumber;
    double m_fpsOfSourceFile;
    double m_currentFPS;
    QString m_CurrentFilename;
    std::shared_ptr<cv::Mat> m_CurrentFrame;

    bool m_Play;
    bool m_Forw;
    bool m_Back;
    bool m_Stop;
    bool m_Paus;


    bool m_TrackingIsActive;


    QString m_NameOfCvMat = "Original";
};

#endif // MEDIAPLAYER_H
