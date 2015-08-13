#pragma once
#include <atomic>
#include <memory>

#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <QThread>
#include <QMouseEvent>

#include "source/util/SharedOpenGlContext.h"
#include "source/util/MutexWrapper.h"

#include "source/core/TextureObject.h"
#include "source/core/TrackingAlgorithm.h"
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

    void initializeOpenGL(std::unique_ptr<Util::SharedOpenGLContext>&& context, TextureObject &texture);

    TrackerStatus getStatus() const
    {
        //TODO maybe lock this part?
        return m_status;
    }

    Mutex &getContextNotCurrent(){
        return m_contextNotCurrentMutex;
    }

    void requestContext(){
        m_context->doneCurrent();
        m_context->moveToThread(QApplication::instance()->thread());
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
	 * Opens a video device
	 */
	void openCamera(int device);

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
    Mutex m_captureActiveMutex;
    Mutex m_readyForNexFrameMutex;
    Mutex m_trackerMutex;
	Mutex m_contextNotCurrentMutex;

	/**
	* Video handling.
	*/
    std::unique_ptr<ImageStream> m_imageStream;

	//defines whether to use pictures as source or a video
    bool m_captureActive GUARDED_BY(m_captureActiveMutex);
    bool m_readyForNextFrame GUARDED_BY(m_readyForNexFrameMutex);
    TrackerStatus m_status;
    std::atomic<bool> m_videoPause;
    bool m_trackerActive;
    double m_fps;
    double m_runningFps;
    bool m_maxSpeed;
    GUIPARAM::MediaType m_mediaType;


    Settings& m_settings;

    QOffscreenSurface m_surface;
    std::unique_ptr<Util::SharedOpenGLContext> m_context;
    TextureObject *m_texture;

    std::shared_ptr<TrackingAlgorithm> m_tracker GUARDED_BY(m_trackerMutex);

	QOpenGLDebugLogger m_openGLLogger;

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

private slots:
	void handleLoggedMessage(const QOpenGLDebugMessage &debugMessage);

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
