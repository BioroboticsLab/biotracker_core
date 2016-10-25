#pragma once

#include <memory>
#include <exception>

#include <boost/filesystem.hpp>

#include <QObject>
#include <QKeyEvent>
#include <QMouseEvent>

#include "util/platform.h"
#include "TrackingThread.h"
#include "ImageStream.h"
#include "Registry.h"
#include "settings/Settings.h"
#include "TrackerStatus.h"
#include "PanZoomState.h"

namespace BioTracker {
namespace Core {
/**
 *@page Requirements Requirements BioTrackerApp [11xx and 12xx]
 *
 *
 * @section sreq Satisfied Requirements 11xx
 *
 * -# Die BioTrackerApp muss dem TrackingThread durch einen SLOT automatisch ermöglichen,
 *      -# @anchor REQ_1111 REQ-1111
 *          die aktuelle frameNumber mit filename und aktuellen fps zu übermitteln.
 *          @see BioTracker::Core::BioTrackerApp::frameCalculatedFromTrackingThread()
 * -# Die BioTrackerApp muss einem receiver (e.g. GUI) ermöglichen, durch eine SIGNAL automatisch
 *      -# @anchor REQ_1112 REQ-1112 die aktuelle frameNumber mit filename und aktuellen fps zu erhalten.
 *          @see BioTracker::Core::BioTrackerApp::frameCalculated()
 *
 * -# Die BioTrackerApp muss dem TrackingThread durch einen SLOT automatisch ermöglichen,
 *      -# @anchor REQ_1113 REQ_1113 Benachrichtigungen zu übermitteln.
 *         @see BioTrackerApp::notifyFromTrackingThread()
 * -# Die BioTrackerApp muss einem receiver (e.g. GUI) ermöglichen, durch eine SIGNAL automatisch
 *      -# @anchor REQ_1114 REQ_1114 Benachrichtigungen zu erhalten.
 *         @see BioTrackerApp::notify()
 *
 * -# Die BioTrackerApp muss dem TrackingThread durch einen SLOT automatisch ermöglichen,
 *      -# @anchor REQ_1115 REQ_1115 Informationen über ein neu geladenes InputFile zu übermitteln.
 *         Die Information enthält den Namen, die Anzahl der Frames sowie die fps des Files.
 *         @see BioTrackerApp::fileOpenedFromTrackingThread()
 * -# Die BioTrackerApp muss einem receiver (e.g. GUI) ermöglichen, durch eine SIGNAL automatisch
 *      -# @anchor REQ_1116 REQ_1116 Informationen über ein neu geladenes InputFile zu erhalten.
 *          Die Information enthält den Namen, die Anzahl der Frames sowie die fps des Files.
 *         @see BioTrackerApp::fileOpened()
 *
 * -# Die BioTrackerApp muss dem TrackingThread durch einen SLOT automatisch ermöglichen,
 *      -# @anchor REQ_1117 REQ_1117 einen neu geladenen TrackingAlgorithm zu übermitteln.
 *         @see BioTrackerApp::trackerSelectedFromTrackingThread()
 * -# Die BioTrackerApp muss einem receiver (e.g. GUI) ermöglichen, durch eine SIGNAL automatisch
 *      -# @anchor REQ_1118 REQ_1118 einen neu geladenen TrackingAlgorithm zu erhalten.
 *         @see BioTrackerApp::trackerSelected()
 *
 * -# Die BioTrackerApp muss dem TrackingThread durch einen SLOT automatisch ermöglichen,
 *      -# @anchor REQ_1119 REQ_1119 ein GUI-update-requst zu übermitteln.
 *         @see BioTrackerApp::requestPaintFromTrackingThread()
 * -# Die BioTrackerApp muss einem receiver (e.g. GUI) ermöglichen, durch eine SIGNAL automatisch
 *      -# @anchor REQ_1120 REQ_1120 ein GUI-update-requst zu erhlaten.
 *         @see BioTrackerApp::requestPaint()
 *
 * -# Die BioTrackerApp muss einem Sender durch einen SLOT automatisch ermöglichen,
 *      -# @anchor REQ_1121 REQ_1121 einen neuen Ort mit Trackerbibliotheken zu übermitteln.
 *         @see BioTrackerApp::loadModulesInPath()
 *
 * -# Die BioTrackerApp muss jedem Sender durch einen SLOT automatisch ermöglichen,
 *      -# @anchor REQ_1122 REQ_1122 ein QMouseEvent zu übermitteln.
 *         @see BioTrackerApp::mouseEvent()
 *
 * -# Die BioTrackerApp muss jedem Sender durch einen SLOT automatisch ermöglichen,
 *      -# @anchor REQ_1123 REQ_1123 ein QWheelEvent zu übermitteln.
 *         @see BioTrackerApp::mouseWheelEvent()
 *
 * -# Die BioTrackerApp muss jedem Sender durch einen SLOT automatisch ermöglichen,
 *      -# @anchor REQ_1124 REQ_1124 ein QKeyEvent zu übermitteln.
 *         @see BioTrackerApp::keyboardEvent()
 *
 *
 * @section ureq Unsatisfied Requirements 12xx
 *
 * -# Die BioTrackerApp muss jedem Sender durch einen SLOT automatisch ermöglichen,
 *      -# @anchor REQ_1211 REQ_1211 Benachrichtigungen zu übermitteln.
 *         @see BioTrackerApp::notifyFromTrackingThread() BioTrackerApp::trackerIsAlreadyLoadedFromRegistry()
 *
 * -# Die BioTrackerApp muss jedem Sender durch einen SLOT automatisch ermöglichen,
 *      -# @anchor REQ_1212 REQ_1212 jedes QEvent zu übermitteln.
 *         @see BioTrackerApp::mouseEvent()
 *
 * -# Die BioTrackerApp die Fähigkeit besitzen,
 *      -# @anchor REQ_1213 REQ_1213 einen oder mehrere Trackerbibliotheken aus einem Pfad in die Registry zu laden.
 *         @see BioTrackerApp::loadModulesInPath()
 *
 */


/**
 * @class BioTrackerApp
 * @brief The BioTrackerApp profides the CORE API of the BioTracker and is the entry point for useres / GUI.
 *
 * BioTrackerApp acts as a <a href="https://en.wikipedia.org/wiki/Facade_pattern">Facade Pattern</a>.
 * Through the BioTrackerApp class the user or particularly the GUI are able to interact with
 * some BioTracker::Core classes like Settings, the Registry and the TrackingThread.
 *
 * Behind the scenes, BioTrackerApp uses the QObject class for communication.
 * QObject is the heart of the Qt Object Model. The central feature in this model is a
 * very powerful mechanism for seamless object communication called
 * <a href="http://doc.qt.io/qt-5/signalsandslots.html">Signals and Slots</a>.
 *
 *
 * @subsection unsatisfiedreq Unsatisfied Requirements
 *
 * -# Die BioTrackerApp ist fähig Views vom TrackingThread mitgeteilt zu bekommen. Außerdem ist reicht er
 *      sie über ein SIGNAL weiter.
 * @note was ist der Sinn von TrackingThread::registerViewsFromTracker() und BioTrackerApp::registerViews()
 *
 */
/* Used to be Facade */
class BIOTRACKER_DLLEXPORT BioTrackerApp : public QObject {
  public:
    Q_OBJECT
  public:
    /**
     * @brief This is the constructor.
     *
     */
    BioTrackerApp();

    /**
     * @brief This is the destructor.
     */
    ~BioTrackerApp() override;

    /**
     * @brief This function initiates the Signals and Slots connections.
     *
     */
    void initConnects();

    /**
     * @brief This function returns a reference to the Settings object.
     * @return Settings
     */
    Settings &getSettings() {
        return m_settings;
    }

    /**
     * @brief This function returns a reference to the Registry object.
     * @return Registry
     */
    Registry &getRegistry() {
        return m_registry;
    }


    /**
     * @brief This function returns a reference to the TrackingThread object.
     * @return TrackingThread
     */
    TrackingThread &getTrackingThread() {
        return m_trackingThread;
    }


    /**
     * @brief This function returns the status TrackerStatus of the TrackingThread.
     * The TrackerStatus enum describes the current status of the tracking algorithm
     * @see TrackerStatus
     * @return TrackerStatus
     */
    TrackerStatus getStatus() const {
        return m_trackingThread.getStatus();
    }

    /**
     * @throw boost::filesystem_error
     * @brief This function opens a single video
     * @param path Path to the
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
     * @brief enableTracking: trackers will paint and track
     */
    void enableTracking();

    /**
     * @brief disableTracking: trackers will paint but not track
     */
    void disableTracking();

    /**
     * @brief paint
     */
    void paint(const size_t, const size_t, QPainter &painter, PanZoomState,
               TrackingAlgorithm::View const & = TrackingAlgorithm::OriginalView);

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
     * @param trackingAlgorithm the selected algorithm
     */
    void setTrackingAlgorithm(std::shared_ptr<TrackingAlgorithm> trackingAlgorithm);
    void setTrackingAlgorithm(std::string const &trackerName);

    /**
     * @brief This function receives a QMouseEvent and passes the event to TrackingThread::mouseEvent().
     *
     * @note This function could be replaced by a more general function in order to realice a Interaction Framework.
     *
     * @satisfy{@req{1122}}
     * @open{@req{1212}}
     */
    void mouseEvent(QMouseEvent *event);

    /**
     * @brief This function receives a QWheelEvent and passes the event to TrackingThread::mouseWheelEvent().
     *
     * @note This function could be replaced by a more general function in order to realice a Interaction Framework.
     *
     * @satisfy{@req{1123}}
     * @open{@req{1212}}
     */
    void mouseWheelEvent(QWheelEvent *event);

    /**
     * @brief This function receives a QKeyEvent and passes the event to TrackingThread::keyboardEvent().
     *
     * @note This function could be replaced by a more general function in order to realice a Interaction Framework.
     *
     * @satisfy{@req{1124}}
     * @open{@req{1212}}
     */
    void keyboardEvent(QKeyEvent *event);

  private Q_SLOTS:
    /**
     * @brief Through this SLOT the BioTrackerApp is able to receive automaticaly messages from TrackingThread.
     *
     * @param message This is the string containing the message.
     * @param type This is the message type. Messages::MessageType
     *
     * @bug This SLOT cloud be replaced by a gener SLOT for all notifications comming from any sender.
     * The same goes for the function trackerIsAlreadyLoadedFromRegistry().
     *
     * @satisfy{@req{1113}}
     *
     * This function will be obsolete with this requirement satisfied.
     * @open{@req{1211}}
     */
    void notifyFromTrackingThread(const std::string &message,
                                  const MessageType type);

    /**
     * @brief Through this SLOT the BioTrackerApp is able to receive automaticaly messages from Registry.
     *
     * @bug This SLOT cloud be replaced by a gener SLOT for all notifications comming from any sender.
     * The same goes for the function notifyFromTrackingThread().
     *
     * This function will be obsolete with this requirement satisfied.
     * @open{@req{1211}}
     *
     */
    void trackerIsAlreadyLoadedFromRegistry(const std::string name);

    /**
     * @brief Through this SLOT the BioTrackerApp is able to recieve automaticaly information about
     * the change of an InputFile from TrackingThread. The Information is the fileName, frameNumber and the fps.
     *
     * @param fileName Path to the new file.
     * @param numFrame The number of frames.
     * @param ftps The fps of the of the file.
     *
     * @satisfy{@req{1115}}
     *
     */
    void fileOpenedFromTrackingThread(const std::string fileName,
                                      const size_t numFrame, const double tfps);

    /**
     * @brief Durch diesen SLOT erhält die BioTrackerApp automatisch die aktuelle frameNumber mit filename und
     * aktuellen fps vom TrackingThread.
     *
     * @param frameNumber The frame number of the calculated frame.
     * @param filename The filename of the current frame.
     * @param currentFps The actual fps of the calculation.
     *
     * @note Status messages for the user interface
     * When currentFps = -1, we are not running the video stream but rather
     * just clicked next or previous
     *
     * @satisfy{@req{1111}}
     */
    void frameCalculatedFromTrackingThread(const size_t frameNumber,
                                           const std::string filename, const double currentFps);

    /**
     * @brief Through this SLOT the BioTrackerApp is able to receive automaticaly a new TrackingAlgorithm if it got changed
     * in the TrackingThread.
     *
     * @param a This is a pointer to a TrackingAlgorithm.
     *
     * @satisfy{@req{1117}}
     */
    void trackerSelectedFromTrackingThread(std::shared_ptr<TrackingAlgorithm> a);

    /**
     * @brief Through this SLOT BioTrackerApp is ability to recieve a path with
     * one or more trackerlibraies located in.
     * This function also adds the trackerlibraries to the registry
     *
     * @bug The functinality of adden trackerlibraries to the registry could be decoupled from this SLOT.
     *
     * @param path Path to the location of one or more trackers.
     *
     *
     * @satisfy{@req{1121}}
     * @open{@req{1213}}
     */
    void loadModulesInPath(const boost::filesystem::path &path);

    /** @bug The Idea of this connection is not clear.
     * QObject::connect(&m_trackingThread, &Core::TrackingThread::registerViews,this, &BioTrackerApp::registerViewsFromTrackingThread).
     * So far this connection just passes a from TrackingThread std::string("Original") to a selection box in the GUI.
     * On it's way it gets replaced by a a std::string("Original")...
     *
     */
    void registerViewsFromTrackingThread(const std::vector<TrackingAlgorithm::View> views);

    /**
     * @brief Through this SLOT the BioTrackerApp is able to receive a request to repaint the GUI.
     *
     * @satisfy{@req{1119}}
     */
    void requestPaintFromTrackingThread();

  Q_SIGNALS:
    /**
     * @brief Through this SIGNAL BioTrackerApp can emmit a Message to a receiver (e.g. the GUI)
     *
     * @param message This string contains the message.
     * @param type The type of the message. Messages::MessageType
     *
     * @satisfy{@req{1114}}
     */
    void notify(const std::string &message, const MessageType type);

    /**
     * @brief This SIGNAL is able to past information about a just opened file to a receiver (e.g. the GUI).
     * This information can consist of a string containing e.g the filename,
     * the total number of frames and the fps of that file.
     *
     * @satisfy{@req{1116}}
     */
    void fileOpened(const std::string message, const size_t numFrame,
                    const double tfps);

    /**
     * @brief Dieses Signal übermittelt die aktuelle frameNumber mit filename und aktuellen fps.
     * @param frameNumber The frame number of the calculated frame.
     * @param filename The filename of the current frame.
     * @param currentFps The actual fps of the calculation.
     *
     * @satisfy{@req{1112}}
     */
    void frameCalculated(const size_t frameNumber, const std::string filename,
                         const double currentFps);

    /** @bug The Idea of this connection is not clear.
     * QObject::connect(&m_trackingThread, &Core::TrackingThread::registerViews,this, &BioTrackerApp::registerViewsFromTrackingThread).
     * So far this connection just passes a std::string("Original") to a selection box in the GUI.
     *
     */
    void registerViews(const std::vector<TrackingAlgorithm::View> views);

    /**
     * @brief trackerSelected
     * Gets thrown when a new Tracker is selected
     * @brief This SIGNAL is able to pass a TrackingAlgorithm to a receiver (e.g. the GUI).
     *
     * @param trackingAlgorithm This is the pointer of the TrackingAlgorithm beeing transmitted.
     *
     * @satisfy{@req{1118}}
     *
     */
    void trackerSelected(std::shared_ptr<TrackingAlgorithm> trackingAlgorithm);

    /**
     * @brief Through this SIGNAL the BioTrackerApp is able to request a receiver (e.g. the GUI) for a GUI-repaint.
     *
     *
     * @satisfy{@req{1120}}
     */
    void requestPaint();

  private:
    Settings m_settings;
    Registry &m_registry;
    TrackingThread m_trackingThread;
};

} // Core
} // BioTracker
