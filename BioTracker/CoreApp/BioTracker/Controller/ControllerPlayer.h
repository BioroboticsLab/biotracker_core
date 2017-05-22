/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/
#ifndef CONTROLLERPLAYER_H
#define CONTROLLERPLAYER_H

#include "Interfaces/IController/IController.h"
#include "Model/MediaPlayer.h"
#include "QPointer"

/**
 * The ControllerPlayer class it the controller of the MediaPlayer-Component. This controller creates and controlls the IModel class MediaPlayer and the IView class VideoControllWidget.
 */
class ControllerPlayer : public IController {
    Q_OBJECT
  public:
    ControllerPlayer(QObject* parent = 0, IBioTrackerContext* context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);
    ~ControllerPlayer();

    /**
     * Hands over the file path of a video to the IModel class MediaPlayer.
     */
    void loadVideoStream(QString str);
    /**
     * Hands over the file path of pictures to the IModel class MediaPlayer.
     */
    void loadPictures(std::vector<boost::filesystem::path> files);
    /**
     * Hands over the camera device number to the IModel class MediaPlayer.
     */
    void loadCameraDevice(int i);

    /**
     * Tells the MediaPlayer-Component to hand over the current cv::Mat and the current frame number to the BioTracker Plugin.
     */
    void setTrackingActivated();
    /**
     * Tells the MediaPlayer-Component not to hand over the current cv::Mat and the current frame number to the BioTracker Plugin.
     */
    void setTrackingDeactivated();

    /**
     * Tells the IModel class MediaPlayer to display the next image frame of the ImageStream.
     */
    void nextFrame();
    /**
     * Tells the IModel class MediaPlayer to display the previous image frame of the ImageStream.
     */
    void prevFrame();
    /**
     * Tells the IModel class MediaPlayer to start playing the ImageStream.
     */
    void play();
    /**
     * Tells the IModel class MediaPlayer to stop playing the ImageStream.
     */
    void stop();
    /**
     * Tells the IModel class MediaPlayer to pause playing the ImageStream.
     */
    void pause();
    /**
     * Tells the IModel class MediaPlayer to jump directly to the specified image frame by the parameter frame.
     */
    void setGoToFrame(int frame);
    /**
     * If the user changes the ImageView in the comboBox represented in the VideoControllWidget it passes the selected ImageView name to the ControllerTextureObject class of the TextureObject-Component.
     */
    void changeImageView(QString str);

    // IController interface
  public:
    void connectControllerToController() override;

  public Q_SLOTS:
    /**
     * This SLOT receives a cv::Mat and a name for the cv::Mat from the MediaPlayer class and hands it over to the ControllerTextureObject for rendering.
     */
    void receiveRenderImage(std::shared_ptr<cv::Mat> mat, QString name);
    /**
     * This SLOT receives a cv::Mat and its frame number and hands it over to the ControllerPlugin for Tracking in the BioTracker Plugin.
     */
    void receiveImageToTracker(std::shared_ptr<cv::Mat> mat, uint number);


  protected:
    void createModel() override;
    void createView() override;
    void connectModelToController() override;
};

#endif // CONTROLLERPLAYER_H
