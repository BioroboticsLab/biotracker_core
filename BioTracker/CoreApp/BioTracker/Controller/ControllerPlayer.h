#ifndef CONTROLLERPLAYER_H
#define CONTROLLERPLAYER_H

#include "Interfaces/IController/IController.h"
#include "Model/MediaPlayer.h"
#include "QPointer"


class ControllerPlayer : public IController {
    Q_OBJECT
  public:
    ControllerPlayer(QObject* parent = 0, IBioTrackerContext* context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);
    ~ControllerPlayer();

    void loadVideoStream(QString str);
    void loadPictures(std::vector<boost::filesystem::path> files);
    void loadCameraDevice(int i);

    void setTrackingActivated();
    void setTrackingDeactivated();

    void nextFrame();
    void prevFrame();
    void play();
    void stop();
    void pause();
    void setGoToFrame(int frame);
    void changeImageView(QString str);

    // IController interface
  public:
    void connectControllerToController() override;

  public Q_SLOTS:
    void receiveRenderImage(std::shared_ptr<cv::Mat> mat, QString name);
    void receiveImageToTracker(std::shared_ptr<cv::Mat> mat, uint number);


  protected:
    void createModel() override;
    void createView() override;
    void connectModelToController() override;
};

#endif // CONTROLLERPLAYER_H
