#pragma once
#include <atomic>
#include <memory>

#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <QThread>
#include <QMouseEvent>

#include "source/core/TrackingAlgorithm.h"
#include "source/util/MutexWrapper.h"
#include "source/core/ImageStream.h"

class Settings;

namespace BioTracker {
namespace Core {

class TrackingThread : public QThread
{
public:
    /**
     * @brief The TrackerStatus enum
     * describes the current status of the tracking algorithm
     */
    enum class TrackerStatus {
        NothingLoaded,  ///< No media selected
        Running,        ///< The tracker is running
        Paused,         ///< The tracker is paused. The calculation of the current frame might still be running
        Invalid         ///< The replayed file is invalid
    };

	Q_OBJECT
public:
	TrackingThread(Settings &settings);
	~TrackingThread(void);
	
    TrackerStatus getStatus() const
    {
        //TODO maybe lock this part?
        return m_status;
    }

	/**
	* Starts the video thread.
	*/
    void loadVideo(const boost::filesystem::path& filename);

	/**
	* Loads in pictures instead of a video
	*/
    void loadPictures(std::vector<boost::filesystem::path>&& filenames);

	/**
	* Checks if thread can handle next frame.
	* @return true if it can, false otherwise.
	*/
	bool isReadyForNextFrame();

	/**	
	* @return  current fps setting
	*/
	double getFps() const;

	/**
	 * @brief stops the currently running thread
	 */
	void stop();

    size_t getVideoLength() const;

    void mouseEvent(QMouseEvent *event);
    void keyboardEvent(QKeyEvent *event);

private:
	Mutex _captureActiveMutex;
	Mutex _readyForNexFrameMutex;
	Mutex _trackerMutex;

	/** 
	* Video handling.
	*/
    std::unique_ptr<ImageStream> _imageStream;
	
	//defines whether to use pictures as source or a video
	bool _captureActive GUARDED_BY(_captureActiveMutex);
	bool _readyForNextFrame GUARDED_BY(_readyForNexFrameMutex);
    TrackerStatus m_status;
	std::atomic<bool> _videoPause;
	bool _trackerActive;
	double _fps;
	double _runningFps;
	bool _maxSpeed;
	GUIPARAM::MediaType _mediaType;

	Settings &_settings;

	std::shared_ptr<TrackingAlgorithm> _tracker GUARDED_BY(_trackerMutex);

	/**
	 * Increments the current frame number by 1 frame.
	 */
	void incrementFrameNumber();
	
	/**
	* Checks if thread is in pause state.
	* @return true if paused, false otherwise.
	*/
	bool isVideoPause() const;

	/**
	* Checks if the tracker is on.
	* @return true if tracker is on, false otherwise.
	*/ 
	bool isCaptureActive();

	/**
	* Set the capture in active or in-active state
	* @param enabled if true capture will be activated, false otherwise
	*/
	void enableCapture(bool enabled);

	/**
	* Initializes the reading capture.
	*/
	void initCaptureForReadingVideoOrStream();

	/**
	* sends frame and everything else that is needed to selected
	* tracking algorithm
	*/
	void doTracking();

	/** 
	* thread running method.
	*/
	virtual void run() override;

public slots:
	void enableHandlingNextFrame(bool nextFrame);
	void enableVideoPause(bool videoPause);

	/**
	* Stops the video.
	*/
    void terminateThread();

	/**
	* Sets the current frame number.
	* @param frameNumber specifies the current frame number.
	*/
    void setFrameNumber(size_t frameNumber);

	/**
	* Gets current frame number.
	* @return the current frame number.
	*/
    size_t getFrameNumber() const;

	/**
	* Moves one frame forward
	*/
	void nextFrame();

	/**
	* change framerate
	*/
	void setFps(double fps);

	/**
	* enable maximum playback speed
	*/
	void setMaxSpeed (bool enabled);

	void setTrackingAlgorithm( std::shared_ptr<TrackingAlgorithm> TrackingAlgorithm );

	void doTrackingAndUpdateScreen();

signals:
	/**
	* Signals when a tracking sequence is done.
	* @param image send the image to draw,
	*/
	void trackingSequenceDone(cv::Mat image);

	/**
	* emit current frame number.
	* @param frameNumber the current frame number.
	*/
	void newFrameNumber(int frameNumber);

	/**
	* singal to gui that video is paused
	*/
	void invalidFile();

	/**
	* send a message to the GUI.
	*/
	void notifyGUI(std::string message, MSGS::MTYPE type = MSGS::MTYPE::NOTIFICATION);
	
	//send current fps to gui
	void sendFps(double fps);
	
	/**
	* display current file name in GUI
	*/
	void fileNameChange(QString filename);
};

}
}
