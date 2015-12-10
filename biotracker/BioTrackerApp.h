#pragma once

#include <memory>
#include <exception>

#include <boost/filesystem.hpp>

#include <QObject>
#include <QKeyEvent>
#include <QMouseEvent>

#include "TrackingThread.h"
#include "ImageStream.h"
#include "Registry.h"
#include "settings/Settings.h"
#include "TrackerStatus.h"

namespace BioTracker {
namespace Core {

/* Used to be Facade */
class BioTrackerApp : public QObject {
  public:
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

    TrackerStatus getStatus() const {
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
     * @brief paint
     * @param device
     * @param painter
     */
    void paint(QPaintDevice &device, QPainter &painter, TrackingAlgorithm::View const & = TrackingAlgorithm::OriginalView);

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
     * @brief setMaxSpeed
     * @param enabled
     */
    void setMaxSpeed(bool enabled);

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
    void setTrackingAlgorithm(std::shared_ptr<TrackingAlgorithm> trackingAlgorithm);
    void setTrackingAlgorithm(std::string const &trackerName);

    void mouseEvent(QMouseEvent *event);
    void mouseWheelEvent(QWheelEvent *event);
    void keyboardEvent(QKeyEvent *event);

  private Q_SLOTS:
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
                                      const size_t numFrame, const double tfps);

    /**
     * @brief notify
     *      Status messages for the user interface
     *      When currentFps = -1, we are not running the video stream but rather
     *      just clicked next or previous
     */
    void frameCalculatedFromTrackingThread(const size_t frameNumber,
                                           const std::string filename, const double currentFps);

    /**
     * @brief trackerSelectedFromTrackingThread
     * forward
     * @param a
     */
    void trackerSelectedFromTrackingThread(std::shared_ptr<TrackingAlgorithm> a);

    /**
     * @brief loadModulesInPath
     *      Dynamically load all modules in path.
     * @param path
     */
    void loadModulesInPath(const boost::filesystem::path &path);

    void registerViewsFromTrackingThread(const std::vector<TrackingAlgorithm::View> views);

  Q_SIGNALS:
    /**
     * @brief notify
     *      Status messages for the user interface
     */
    void notify(const std::string &message, const MSGS::MTYPE type);

    /**
     * @brief fileOpened
     *      Update event for title and slider
     */
    void fileOpened(const std::string message, const size_t numFrame,
                    const double tfps);

    /**
     * @brief frameCalculated
     *      Callback that gets called every time a new frame is calculated
     * @param frameNumber the frame number of the calculated frame
     * @param filename filename of the current frame
     * @param currentFps actual fps of the calculation
     */
    void frameCalculated(const size_t frameNumber, const std::string filename,
                         const double currentFps);

    void registerViews(const std::vector<TrackingAlgorithm::View> views);

    /**
     * @brief trackerSelected
     * Gets thrown when a new Tracker is selected
     * @param trackingAlgorithm
     */
    void trackerSelected(std::shared_ptr<TrackingAlgorithm> trackingAlgorithm);

  private:
    Settings m_settings;
    Registry &m_registry;
    TrackingThread m_trackingThread;
};

} // Core
} // BioTracker
