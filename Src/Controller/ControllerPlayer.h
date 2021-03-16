/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/
#ifndef CONTROLLERPLAYER_H
#define CONTROLLERPLAYER_H

#include "IControllerCfg.h"
#include "Model/MediaPlayer.h"
#include "QPointer"
#include "util/types.h"

/**
 * The ControllerPlayer class it the controller of the MediaPlayer-Component. This controller creates and controlls the IModel class MediaPlayer and the IView class VideoControllWidget.
 */
class ControllerPlayer : public IControllerCfg {
	Q_OBJECT
	public:
		ControllerPlayer(QObject* parent = 0, IBioTrackerContext* context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);
		~ControllerPlayer();

		/**
		* Hands over the file path of a video to the IModel class MediaPlayer.
		*/
		void loadVideoStream(std::vector<boost::filesystem::path> files);
		/**
		* Hands over the file path of pictures to the IModel class MediaPlayer.
		*/
		void loadPictures(std::vector<boost::filesystem::path> files);
		/**
		* Hands over the camera device number to the IModel class MediaPlayer.
		*/
		void loadCameraDevice(CameraConfiguration conf);

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
		* If the user changes the ImageView in the comboBox represented in the VideoControllWidget it passes the selected ImageView name to the ControllerTextureObject class of the TextureObject-Component.
		*/
		void changeImageView(QString str);

		/**
		* User clicked record -> record "what the user sees"
		*/
		int recordOutput();

		int recordInput();

        void setTargetFps(double fps);

	QString takeScreenshot();

	// IController interface
	public:
		void connectControllerToController() override;

	signals:
		void emitPauseState(bool state);
		void signalCurrentFrameNumberToPlugin(uint frameNumber);
        void emitNextMediaInBatch(const std::string path);
        void emitNextMediaInBatchLoaded(const std::string path);
		void emitEndOfPlayback();

	public Q_SLOTS:
		/**
		* This SLOT receives a cv::Mat and a name for the cv::Mat from the MediaPlayer class and hands it over to the ControllerTextureObject for rendering.
		*/
		void receiveRenderImage(std::shared_ptr<cv::Mat> mat, QString name);
		/**
		* This SLOT receives a cv::Mat and its frame number and hands it over to the ControllerPlugin for Tracking in the BioTracker Plugin.
		*/
		void receiveImageToTracker(std::shared_ptr<cv::Mat> mat, uint number);
		/**
		* This SLOT receives a framenumber and hands it over to the ControllerTrackedComponentCore for visualizing in the main app.
		*/
		void receiveVisualizeCurrentModel(uint frameNumber);

		void receiveChangeDisplayImage(QString str);

		void receiveCurrentFrameNumberToPlugin(uint frameNumber);
        /**
        * Tells the IModel class MediaPlayer to jump directly to the specified image frame by the parameter frame.
        */
        void setGoToFrame(int frame);

		void receiveMediumChanged(const std::string path);
		void receiveMaxBatchNumber(int i);
		
        void receiveNextMediaInBatch(const std::string path);
        void receiveNextMediaInBatchLoaded(const std::string path);
		void receiveTrackCount(int trackNo);

	protected:
		void createModel() override;
		void createView() override;
		void connectModelToController() override;

	private:
		int _trackCount = 0;
		int _trackCountEndOfBatch = 0;
};

#endif // CONTROLLERPLAYER_H
