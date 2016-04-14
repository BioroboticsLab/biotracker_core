#pragma once
#include <atomic>
#include <memory>

#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <QThread>
#include <condition_variable>
#include <QMouseEvent>

#include "util/MutexWrapper.h"

#include "TextureObject.h"
#include "TrackingAlgorithm.h"
#include "ImageStream.h"

#include "interpreter/Interpreter.h"
#include "TrackerStatus.h"
#include "PanZoomState.h"

namespace BioTracker {
namespace Core {

class Settings;

class TrackingThread : public QThread {
  public:
    Q_OBJECT
  public:
    TrackingThread(Settings &settings);
    ~TrackingThread(void);

    TrackerStatus getStatus() const {
        //TODO maybe lock this part?
        return m_status;
    }

    /**
    * Loads the last loaded file.
    */
    void loadFromSettings();

    /**
    * Loads a video.
    */
    void loadVideo(const boost::filesystem::path &filename);

    /**
    * Loads in pictures.
    */
    void loadPictures(std::vector<boost::filesystem::path> &&filenames);

    /**
     * Opens a video device.
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
     * @brief paint
     */
    void paint(const size_t, const size_t, QPainter &painter, BioTracker::Core::PanZoomState &z,
               TrackingAlgorithm::View const &);

    /**
     * Checks if the thread is in the rendering stage right now
     */
    bool isRendering() const;

    /**
    * Checks if thread is in pause state.
    * @return true if paused, false otherwise.
    */
    bool isPaused() const;

    /**
     * Toggles the playing state.
     */
    void togglePlaying();

    /**
     * @brief enableTracking
     * enable the tracker to actually track data.
     */
    void enableTracking();

    /**
     * @brief enableTracking
     * sets the tracker to be disabled. Only the draw-functions
     * will be called
     */
    void disableTracking();

    /**
    * @return  current fps setting
    */
    double getFps() const;

    size_t getVideoLength() const;

    TextureObject const &getTexture() const {
        return m_texture;
    }

    void mouseEvent(QMouseEvent *event);
    void mouseWheelEvent(QWheelEvent *event);
    void keyboardEvent(QKeyEvent *event);

  private:
    std::unique_ptr<ImageStream> m_imageStream;
    Mutex m_trackerMutex;
    std::mutex m_tickMutex;
    std::mutex m_paintMutex;
    std::mutex m_renderMutex;
    std::condition_variable m_conditionVariable;
    size_t m_lastFrameNumber;

    bool m_playing;
    bool m_playOnce;
    bool m_isRendering;
    bool m_somethingIsLoaded;

    // flag is needed so that we can enable/disable tracking
    // regardless of the loaded tracker. A tracker most not even
    // be loaded.
    bool m_isTrackingEnabled;

    // flag to determine if the 'filenamechanged' event should
    // occur or not
    bool m_ignoreFilenameChanged;

    // needed to figure out if the filename has changed or not
    std::string m_lastFilename;

    //defines whether to use pictures as source or a video
    TrackerStatus m_status;

    double m_fps;
    bool m_maxSpeed;
    GuiParam::MediaType m_mediaType;

    Settings &m_settings;

    TextureObject m_texture;

    std::shared_ptr<TrackingAlgorithm> m_tracker GUARDED_BY(m_trackerMutex);

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
     * notifies the thread that it can do the next calculation
     * Must be the last paint-call!!!!
     * @brief paintDone
     */
    void paintDone();

    /**
    * Does exactly one tick, eg. drawing one image and starting tracker once.
    */
    void tick(const double fps);

    /**
    * Moves one frame forward
    */
    void nextFrame();

    /**
    * thread running method.
    */
    virtual void run() override;

  public Q_SLOTS:

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
    * change framerate
    */
    void setFps(double fps);

    /**
    * enable maximum playback speed
    */
    void setMaxSpeed(bool enabled);

    void setTrackingAlgorithm(std::shared_ptr<TrackingAlgorithm> TrackingAlgorithm);

    void registerViewsFromTracker(const std::vector<TrackingAlgorithm::View> views);

    void requestPaintFromTracker();

    void requestTrackFromTracker();

    void requestPauseFromTracker(bool pause);

    void notifyGUIFromTracker(std::string m, MessageType type = MessageType::NOTIFICATION);

  Q_SIGNALS:
    /**
    * emit current frame number.
    * @param frameNumber the current frame number.
    * @param filename the current file name
    * @param currentFps the fps of the last frame: when the fps are -1, no fps should be shown
    */
    void frameCalculated(const size_t frameNumber, const std::string filename,
                         const double currentFps);

    /**
     * emit new filename and entire frame quantity
     */
    void fileOpened(const std::string fileName, const size_t numFrame,
                    const double targetFps);

    /**
    * send a message to the GUI.
    */
    void notifyGUI(std::string message,
                   MessageType type = MessageType::NOTIFICATION);

    /**
     * @brief trackerSelected
     * Notify the application that the tracker has changed
     */
    void trackerSelected(std::shared_ptr<TrackingAlgorithm> tracker);

    void registerViews(const std::vector<TrackingAlgorithm::View> views);

    void requestPaint();
};

}
}
