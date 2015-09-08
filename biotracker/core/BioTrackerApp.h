#pragma once

#include <memory>
#include <exception>

#include <boost/filesystem.hpp>

#include <QObject>
#include <QKeyEvent>
#include <QMouseEvent>

#include "biotracker/core/TrackingThread.h"
#include "biotracker/core/ImageStream.h"
#include "biotracker/core/Registry.h"
#include "biotracker/core/settings/Settings.h"

#include "biotracker/util/SharedOpenGlContext.h"


namespace BioTracker {
namespace Core {

/* Used to be Facade */
class BioTrackerApp : public QObject {
    Q_OBJECT
  public:
    BioTrackerApp();

    ~BioTrackerApp() override;

    void initConnects();

    void initializeOpenGL(QOpenGLContext *mainContext, TextureObject &texture);

    Settings &getSettings() {
        return m_settings;
    }

    Registry &getRegistry() {
        return m_registry;
    }

    TrackingThread &getTrackingThread() {
        return m_trackingThread;
    }

    TrackingThread::TrackerStatus getStatus() const {
        return m_trackingThread.getStatus();
    }

    /**
     * @throw boost::filesystem_error
     * @brief openVideo opens a single video which type is supported by opencv and stores path in settings
     */
    void openVideo(const boost::filesystem::path &path);

    /**
     * @throw boost::filesystem_error
     * @brief openImages opens a set of images supported by opencv and stores path in settings
     */
    void openImages(std::vector<boost::filesystem::path> paths);

    /**
     * @brief openCamera opens a video device supported by opencv VideoCapture
     */
    void openCamera(int device);

    /**
     * @brief play starts playing the video
     */
    void play();

    /**
     * @brief pause stops the video at the current frame + 1
     */
    void pause();

    /**
     * @return when True, the thread is still rendering a frame right now, even when
     *  paused
     */
    bool isRendering();

    /**
     * Attention: This boolean does not reflect the actual state in the tracking thread but
     * rather the attempted status; that means, that it might return false, when "pause" is
     * called but the tracking thread might still render a last frame.
     * @brief isRunning
     * @return True if the biotracker is playing, if it set to pause, False is returned.
     */
    bool isRunning();

    /**
     * @brief setFrame (0 .. getNumFrames() - 1) if the input does not meets the
     *   invariant the frame will stay at the current frame
     */
    void setFrame(size_t frameNumber);

    /**
     * @brief setTargetFps
     *      if variant is invalid, input is ignored
     *      Default value is set by the data from the video, if this is not
     *      applicable, INFINITY is selected
     * @param fps (0 .. INFINITY)
     */
    void setTargetFps(double fps);

    /**
     * @brief getTargetFps
     * @return the set FPS of the program
     */
    double getTargetFps() const;

    /**
     * @brief getNumFrames
     * @return the number of frames in the current stream
     */
    size_t getNumFrames() const;

    /**
     * @brief getCurrentFrame
     * @return the current frame number
     */
    size_t getCurrentFrameNumber() const;

    /**
     * @brief setTrackingAlgorithm
     * @param TrackingAlgorithm the selected algorithm
     */
    void setTrackingAlgorithm(std::shared_ptr<TrackingAlgorithm> TrackingAlgorithm);

    void mouseEvent(QMouseEvent *event);
    void keyboardEvent(QKeyEvent *event);

  private slots:
    /**
     * @brief notify
     *      Status messages for the user interface
     */
    void notifyFromTrackingThread(const std::string &message,
                                  const MSGS::MTYPE type);

    /**
     * @brief notify
     *      Status messages for the user interface
     */
    void fileOpenedFromTrackingThread(const std::string fileName,
                                      const size_t numFrame);

    /**
     * @brief notify
     *      Status messages for the user interface
     *      When currentFps = -1, we are not running the video stream but rather
     *      just clicked next or previous
     */
    void frameCalculatedFromTrackingThread(const size_t frameNumber,
                                           const std::string filename, const double currentFps);

  signals:
    /**
     * @brief notify
     *      Status messages for the user interface
     */
    void notify(const std::string &message, const MSGS::MTYPE type);

    /**
     * @brief fileOpened
     *      Update event for title and slider
     */
    void fileOpened(const std::string message, const size_t numFrame);

    /**
     * @brief frameCalculated
     *      Callback that gets called every time a new frame is calculated
     * @param frameNumber the frame number of the calculated frame
     * @param filename filename of the current frame
     * @param currentFps actual fps of the calculation
     */
    void frameCalculated(const size_t frameNumber, const std::string filename,
                         const double currentFps);

  private:
    bool m_isRunning;
    Settings m_settings;
    Registry &m_registry;
    TrackingThread m_trackingThread;
};

} // Core
} // BioTracker
