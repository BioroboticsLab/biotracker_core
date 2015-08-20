#pragma once
#include <atomic>
#include <memory>

#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <QThread>
#include <condition_variable>
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
	 * Pause video plaing.
	 */
    void setPause();

    /**
     * Enables video playing.
     */
    void setPlay();

    /**
     * Toggles the playing state.
     */
    void togglePlaying();

	/**
	* @return  current fps setting
	*/
	double getFps() const;

    size_t getVideoLength() const;

    void mouseEvent(QMouseEvent *event);
    void keyboardEvent(QKeyEvent *event);

private:
    std::unique_ptr<ImageStream> m_imageStream;
    Mutex m_trackerMutex;
    std::mutex m_tickMutex;
    std::condition_variable m_conditionVariable;

    bool m_playing;
    bool m_playOnce;

	//defines whether to use pictures as source or a video
    TrackerStatus m_status;

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
	* Checks if thread is in pause state.
	* @return true if paused, false otherwise.
	*/
	bool isPaused() const;

	/**
	* sends frame and everything else that is needed to selected
	* tracking algorithm
	*/
	void doTracking();

    /**
     * Play and calculate the next frame only.
     */
    void playOnce();

	/**
 	* Does exactly one tick, eg. drawing one image and starting tracker once.
 	*/
	void tick();

	/**
	* thread running method.
	*/
	virtual void run() override;

public slots:

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
	//void newFrameNumber(int frameNumber);
    void showFrame(int frameNumber);

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
