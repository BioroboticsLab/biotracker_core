#pragma once

#include <memory>
#include <exception>

#include <boost/filesystem.hpp>

#include <QObject>
#include <QKeyEvent>
#include <QMouseEvent>

#include "source/core/TrackingThread.h"
#include "source/core/ImageStream.h"
#include "source/core/Registry.h"
#include "source/core/settings/Settings.h"

namespace BioTracker {
namespace Core {

class Facade : public QObject {
Q_OBJECT
public:

    Facade();

    ~Facade() override;

    Settings& getSettings() {
        return m_settings;
    }

    Algorithms::Registry& getRegistry() {
        return m_registry;
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
     * @brief play starts playing the video
     */
    void play();

    /**
     * @brief pause stops the video at the current frame + 1
     */
    void pause();

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
    void setTrackingAlgorithm( std::shared_ptr<TrackingAlgorithm> TrackingAlgorithm );

    void mouseEvent(QMouseEvent *event);
    void keyboardEvent(QKeyEvent *event);

signals:

    /**
     * @brief unknownError
     *      handles Errors that are not handled by other signales
     */
    void unknownError(const std::exception_ptr err);

    /**
     * @brief fileError
     *      handles file errors that appear inside of the tracking thread
     */
    void fileError(const boost::filesystem::filesystem_error &err);

    /**
     * @brief notify
     *      Status messages for the user interface
     */
    void notify(const std::string &message, const MSGS::MTYPE type);

    /**
     * @brief frameCalculated
     *      Callback that gets called every time a new frame is calculated
     * @param frameNumber the frame number of the calculated frame
     * @param filename filename of the current frame
     * @param currentFps actual fps of the calculation
     */
    void frameCalculated(const size_t frameNumber, const std::string &filename, const double currentFps);

private:
    Settings m_settings;
    Algorithms::Registry &m_registry;
    TrackingThread m_trackingThread;
};

} // Core
} // BioTracker
