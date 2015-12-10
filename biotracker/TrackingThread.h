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

class Settings;

namespace BioTracker {
namespace Core {

class TrackingThread : public QThread {
  public:
    Q_OBJECT
  public:
    TrackingThread(Settings &settings);
    ~TrackingThread(void);

    void initializeOpenGL(QOpenGLContext *context,
                          TextureObject &texture);

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
     * @param device
     * @param painter
     */
    void paint(QPaintDevice &device, QPainter &painter, TrackingAlgorithm::View const &);

    /**
     * Checks if the thread is in the rendering stage right now
     * @return
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
    * @return  current fps setting
    */
    double getFps() const;

    size_t getVideoLength() const;

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

    bool m_playing;
    bool m_playOnce;
    bool m_isRendering;
    bool m_somethingIsLoaded;

    //defines whether to use pictures as source or a video
    TrackerStatus m_status;

    double m_fps;
    double m_runningFps;
    bool m_maxSpeed;
    GUIPARAM::MediaType m_mediaType;

    Settings &m_settings;

    TextureObject *m_texture;

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
     * Paints the raw texture of the data of the current frame
     * @brief paintRaw
     */
    void paintRaw();

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
                   MSGS::MTYPE type = MSGS::MTYPE::NOTIFICATION);

    /**
     * @brief trackerSelected
     * Notify the application that the tracker has changed
     * @param tracker
     */
    void trackerSelected(std::shared_ptr<TrackingAlgorithm> tracker);

    void registerViews(const std::vector<TrackingAlgorithm::View> views);

    void requestPaint();
};

}
}
