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

/**
 *@page TTRequirements Requirements TrackingThread [15xx and 16xx]
 *
 * -# @anchor REQ_1511 REQ-1511 <BR>
 * Ist fähig, gespeicherte Settings vom default Config File zu laden.
 * <BR> - TrackingThread::loadFromSettings()
 *
 * -# @anchor REQ_1512 REQ-1512 <BR>
 * bietet die Möglichkeit, ein Video von einem Filepath zu laden.
 * <BR> - TrackingThread::loadVideo()
 *
 * -# @anchor REQ_1513 REQ-1513 <BR>
 * bietet die Möglichkeit, ein oder mehrere Bilder von einem Filepath zu laden.
 * <BR> - TrackingThread::loadPictures()
 *
 * -# @anchor REQ_1514 REQ-1514 <BR>
 * bietet die Möglichkeit, ein camera zu laden.
 * <BR> - TrackingThread::loadCamera()
 *
 * -# @anchor REQ_1515 REQ-1515 <BR>
 * bietet die Möglichkeit, seinen Status auf Pause zu setzten.
 * <BR> - TrackingThread::setPause()
 *
 * -# @anchor REQ_1516 REQ-1516 <BR>
 * bietet die Möglichkeit, seinen Status auf Play zu setzten.
 * <BR> - TrackingThread::setPlay()
 *
 * -# @anchor REQ_1517 REQ-1517 <BR>
 * bietet die Möglichkeit, auf ProxMat zu zeichnen.
 * @note Fähigkeit ist noch nicht klar definiert.
 * <BR> - TrackingThread::paint()
 *
 * -# @anchor REQ_1518 REQ-1518 <BR>
 * bietet die Möglichkeit, abzufragen ob gerade ein Rendering stattfindet.
 * <BR> - TrackingThread::isRendering()
 *
 * -# @anchor REQ_1519 REQ-1519 <BR>
 * bietet die Möglichkeit, abzufragen ob sein Status momentan Pause ist.
 * <BR> - TrackingThread::isPaused()
 *
 * -# @anchor REQ_1520 REQ-1520 <BR>
 * bietet die Möglichkeit, auf Pause und gleich danach wieder auf Paly zu setzten.
 * @note dies kann doch schon mit @req{1516} und @req{1515} gemacht werden.
 * Diese Funktion kann man wohl abschalten.
 * <BR> - TrackingThread::togglePlaying()
 *
 * -# @anchor REQ_1521 REQ-1521 <BR>
 * bietet die Möglichkeit, das tracking im TrackingAlgorithm zu aktivieren und gleichzeitig
 * den Status im TrackingThread auf tracking zu setzten.
 * @note diese beiden Funktionalitäten könnten entkoppelt werden.
 * <BR> - TrackingThread::enableTracking()
 *
 * -# @anchor REQ_1522 REQ-1522 <BR>
 * bietet die Möglichkeit, das tracking im TrackingAlgorithm zu deaktivieren und gleichzeitig
 * den Status im TrackingThread auf nichttracking zu setzten.
 * @note diese beiden Funktionalitäten könnten entkoppelt werden.
 * <BR> - TrackingThread::disableTracking()
 *
 * -# @anchor REQ_1523 REQ-1523 <BR>
 * bietet die Möglichkeit, die aktuelle FPS abzurufen.
 * <BR> - TrackingThread::getFps()
 *
 * -# @anchor REQ_1524 REQ-1524 <BR>
 * bietet die Möglichkeit, die Anzahl der Frames des aktuellen Videos abzurufen.
 * <BR> - TrackingThread::getFps()
 *
 * -# @anchor REQ_1525 REQ-1525 <BR>
 * bietet die Möglichkeit, das aktuelle TextureObject abzurufen.
 * <BR> - TrackingThread::getTexture()
 *
 * -# @anchor REQ_1526 REQ-1526 <BR>
 * hat die Fähigkeit, ein QEvent an den TrackingAlgorithm weiter zu geben.
 * @open{req@{1526}}
 *
 *
 * -# @anchor REQ_1527 REQ-1527 <BR>
 * Wenn ein QEvent an den TrackingAlgorithm übergeben wird, muss der TrackingThread
 * sicher stellen, dass TrackingAlgorithm in seinem Thread für die Übergabe gesperrt ist.
 * <BR> - TrackingThread::m_trackerMutex
 *
 * -# @anchor REQ_1528 REQ-1528 <BR>
 * bietet die Möglichkeit, ein QMouseEvent an den TrackingAlgorithm weiter zu geben.
 * <BR> - TrackingThread::mouseEvent()
 *
 * -# @anchor REQ_1529 REQ-1529 <BR>
 * bietet die Möglichkeit, ein QWheelEvent an den TrackingAlgorithm weiter zu geben.
 * <BR> - TrackingThread::mouseWheelEvent()
 *
 * -# @anchor REQ_1530 REQ-1530 <BR>
 * bietet die Möglichkeit, ein QKeyEvent an den TrackingAlgorithm weiter zu geben.
 * <BR> - TrackingThread::keyboardEvent()
 *
 *
 * -# @anchor REQ_1531 REQ-1531 <BR>
 * muss fähig sein, den Tracker TrackingAlgorithm durch einen Mutix zu sichern.
 * <BR> - TrackingThread::m_trackerMutex
 *
 * -# @anchor REQ_1532 REQ-1532 <BR>
 * TrackingThread bietet Sendern die Möglichkeit, durch einen SLOT automatisch
 * eine bestimmte FrameNumber im TrackingAlgorithm zu tracken.
 * <BR> - TrackingThread::setFrameNumber()
 *
 * -# @anchor REQ_1533 REQ-1533 <BR>
 * TrackingThread bietet die Möglichkeit, die aktuelle frameNumber abzufragen.
 * <BR> - TrackingThread::getFrameNumber()
 *
 * -# @anchor REQ_1534 REQ-1534 <BR>
 * TrackingThread provides a sender with the ability to submit a new speed (fps)
 * for a Video, that will be passed over to TrackingAlgorithm.
 * <BR> - TrackingThread::setFps(double fps)
 *
 * -# @anchor REQ_1535 REQ-1535 <BR>
 * provides a sender with the ability to run the tracking TrackingAlgorithm
 * with maximum speed possible. It also provides the ability to run the tracking not with
 * the maxiumm speed.
 * <BR> - TrackingThread::setMaxSpeed()
 * @note What happens if set MaxSpeed is set to false. I think such a function should only be a
 * trigger to get to the maximum speed. The undoing of max. speed should be done somewhere else.
 *
 *
 * -# @anchor REQ_1536 REQ-1536 <BR>
 * is able to run the tracking TrackingAlgorithm with not more then the currently defined fsp.
 * <BR> - TrackingThread::run()
 *
 * -# @anchor REQ_1537 REQ-1537 <BR>
 * @copybrief TrackingThread::setTrackingAlgorithm()
 * <BR> - TrackingThread::setTrackingAlgorithm() <BR>
 *
 * -# @anchor REQ_1538 REQ-1538 <BR>
 * provides a sender with the ability to submit one or more views TrackingAlgorithm::View.
 * <BR> - TrackingThread::registerViewsFromTracker()
 *
 * -# @anchor REQ_1539 REQ-1539 <BR>
 * provides a sender (e.g. TrackingAlgorithm) with the ability to request a repaint.
 * <BR> - TrackingThread::requestPaintFromTracker()
 *
 * -# @anchor REQ_1540 REQ-1540 <BR>
 * provides a sender (e.g. TrackingAlgorithm) with the ability to send a message string and it's
 * MessageType in order to pass it on to a receiver of TrackingThread::notifyGUI().
 * <BR> - TrackingThread::notifyGUIFromTracker()
 *
 * -# @anchor REQ_1541 REQ-1541 <BR>
 * has the ability to automatically send the current frameName, the frameNumber and the fps
 * to a receiver of the SIGNAL TrackingThread::frameCalculated()
 * <BR> - TrackingThread::frameCalculated()
 *
 * -# @anchor REQ_1542 REQ-1542 <BR>
 * @copybrief TrackingThread::fileOpened()
 * <BR> - TrackingThread::fileOpened()
 *
 * -# @anchor REQ_1543 REQ-1543 <BR>
 * @copybrief TrackingThread::notifyGUI()
 * <BR> - TrackingThread::notifyGUI()
 *
 * -# @anchor REQ_1544 REQ-1544 <BR>
 * @copybrief TrackingThread::trackerSelected()
 * <BR> - TrackingThread::trackerSelected()
 *
 * -# @anchor REQ_1545 REQ-1545 <BR>
 * @copybrief TrackingThread::registerViews()
 * <BR> - TrackingThread::registerViews()
 *
 * -# @anchor REQ_1546 REQ-1546 <BR>
 * @copybrief TrackingThread::requestPaint()
 * <BR> - TrackingThread::requestPaint()
 *
 * @section ttsreq Satisfied Requirements 15xx
 *
 *
 * @section ttureq Unsatisfied Requirements 16xx
 *
 * -# @anchor REQ_1611 REQ-1611 <BR>
 * TrackingThread must provide a fuction that toggles between paly and pause.
 *
 *
 *
 */


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
    * @satisfy{@req{1511}}
    */
    void loadFromSettings();

    /**
     * @brief This function loads the video from path of filename and saves
     * the path to Settings.
     * @param filename
     * @return void
     *
     * @satisfy{@req{1512}}
     */
    void loadVideo(const boost::filesystem::path &filename);

    /**
     * @brief This function loads from pictures.
     * @param filename
     * @return void
     *
     * @satisfy{@req{1513}}
     */
    void loadPictures(std::vector<boost::filesystem::path> &&filenames);

    /**
     * Opens a video device.
     *
     * @satisfy{@req{1514}}
     */
    void loadCamera(int device);

    /**
     * Pause video plaing.
     *
     * @satisfy{@req{1515}}
     */
    void setPause();

    /**
     * Enables video playing.
     *
     * @satisfy{@req{1516}}
     */
    void setPlay();

    /**
     * @brief paint
     *
     * @satisfy{@req{1517}}
     */
    void paint(const size_t, const size_t, QPainter &painter, BioTracker::Core::PanZoomState &z,
               TrackingAlgorithm::View const &);

    /**
     * Checks if the thread is in the rendering stage right now
     *
     * @satisfy{@req{1518}}
     */
    bool isRendering() const;

    /**
    * Checks if thread is in pause state.
    * @return true if paused, false otherwise.
    *
    * @satisfy{@req{1519}}
    */
    bool isPaused() const;

    /**
     * Toggles the playing state.
     * @satisfy{@req{1520}}
     */
    void togglePlaying();

    /**
     * @brief enableTracking
     * enable the tracker to actually track data.
     *
     * @satisfy{@req{1521}}
     */
    void enableTracking();

    /**
     * @brief disableTracking
     * sets the tracker to be disabled. Only the draw-functions
     * will be called
     *
     * @satisfy{@req{1522}}
     */
    void disableTracking();

    /**
    * @return  current fps setting
    *
    * @satisfy{@req{1523}}
    */
    double getFps() const;

    /**
    * @return  Number of frames in the video
    *
    * @satisfy{@req{1524}}
    */
    size_t getVideoLength() const;

    /**
    * @return  TextureObject A reverence to the current TextureObject
    *
    * @satisfy{@req{1525}}
    */
    TextureObject const &getTexture() const {
        return m_texture;
    }

    /**
    * This function sends an QMouseEvent to the TrackingAlgorithm.
    * While sending the Event, this function locks the mutex for the TrackingAlgorithm.
    *
    * @satisfy{@req{1528}}
    * @satisfy{@req{1527}}
    */
    void mouseEvent(QMouseEvent *event);

    /**
    *
    *
    * @satisfy{@req{1529}}
    * @satisfy{@req{1527}}
    */
    void mouseWheelEvent(QWheelEvent *event);

    /**
    *
    *
    * @satisfy{@req{1530}}
    * @satisfy{@req{1527}}
    */
    void keyboardEvent(QKeyEvent *event);

  private:
    std::unique_ptr<ImageStream> m_imageStream;
    Mutex m_trackerMutex;
    std::mutex m_tickMutex;
    std::mutex m_paintMutex;
    std::mutex m_renderMutex;
    std::condition_variable m_conditionVariable;
    size_t m_lastFrameNumber;
    size_t m_lastTrackedFrameNumber; // the last frame number that was tracked

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

    /**
     * @satisfy{@req{1530}}
     *
     */
    std::shared_ptr<TrackingAlgorithm> m_tracker GUARDED_BY(m_trackerMutex);

    /**
    * sends frame and everything else that is needed to selected
    * tracking algorithm
    *
    *
    * @brief -# Die Funktion wird nicht im MainThread sonder im separten Thread für tracking ausgeführt.
    * -# Befor diese Funktion ein Tracking im Tracker auslöst, lockt sie mit einem MutexLocker m_trackerMutex den m_tracker TrackingAlgorithm.
    * -# Der MutexLocker wird mit Beendigung dieser Funktion wieder gelöst.
    * -# Diese Funktion weist den Tacker TrackingAlgorithm an, ein Tracking für ein Bild durchzuführen.
    * <BR> TrackingAlgorithm::attemptTracking()
    * -# Dabei ist sie verantwortlich, dem Tracker ein Bild und dessen FrameNumber zu übermitteln.
    * -# Wenn beim Aufruf zum Tracking im TrackingAlgorithm ein Fehler auftritt, ist diese Funktion
    * Fähigkeit, eine exception vom Typ MessageType::FAIL zu schmeißen.
    * -# Diese Funktion hat die Fähigkeit, die FrameNumber des Frames, den sie zum Tracker übermittelt hatte
    * als Member zu speichern. Der Grund dafür ist, dass die Framenumber in weitern TrackingThread Methoden
    * verwendet wird und deshalb sicher gestellt werden muss, dass diese keinen race conditionen ausgesetzt wird.
    *
    */
    void doTracking();

    /**
     * @brief TrackingThread::playOnce
     *
     * -# Diese Funktion übergibt dem TextureObject m_texture den aktuellen Frame aus dem ImageStream.
     * -# Sie setzt den Zustand des Trackers auf m_playOnce und m_somethingIsLoaded.
     * -# Sie wird immer dann aufgerufen, wenn Video, Picture oder Camara neu geladen werden oder
     * wenn ein bestimmter Frame manuell (durch Stopptaste=0 oder druch steppen druchs Bild)
     * abgerufen wird.
     * -# Sie gibt das Signal 'notify_all()' an m_conditionVariable in der Methode run(), die den Thread so lange warten lässt,
     * bis dieses Signal gegeben wurde.
     *
     */
    void playOnce();

    /**
     *
     * @brief @brief This function checks if something is loaded m_somethingIsLoaded.
     * Falls m_somethingIsLoaded verlässt TrackingThread den Zustand isRendering m_isRendering.
     * Sobald isRendering verlassen wurde wird die conditionVariable in
     * der run() Methode benachrichtigt m_conditionVariable.notify_all().
     *
     * @details notifies the thread that it can do the next calculation
     * Must be the last paint-call!!!!
     */
    void paintDone();

    /**
     * @brief -# Soblad TrackingThread::tick() aufgerufen wird, die Methode mit dem Mutex m_renderMutex gesichert.
     * -# Solange die Methode gelockt ist wird der FileName des aktuellen Frames aus dem Imagestream geladen.
     * -# Falls TrackingThread nicht im Zustand ignoreFilenameChanged ist und falls der vorhergehende
     * FrameName ungleich dem aktuelle FrameName ist
     *     -# und falls ein Tracker geladen ist, wird der TrackingAlgorithm::onFileChanged(string) benachrichtigt.
     *     -# wird der aktuelle FileName als lastFileName m_lastFilename gespeichert.
     * -# TrackingThread ruft die Methode TrackingThread::doTracking() auf.
     * -# Sobald TrackingThread::doTracking() beendet wurde, wird die aktuelle FrameNumber aus dem ImageStream::currentFrameNumber()
     * abgefragt und lokal gespeichert.
     * -# Falls TrackingThread im Zustand isPlaying m_isPlaying ist, wird die Methode TrackingThread::nextFrame()
     * aufgerufen.
     * -# TrackingThread löst den Mutex m_renderMutex.
     * -# Soblad m_renderMutex gelöst ist, lockt TrackingThread den Mutex m_paintMutex.
     * -# Sobald m_paintMutex gelockt ist, wird das SIGNAL frameCalculated() ausgestoßen.
     * -# Sobald das SIGNAL frameCalculated() ausgestoßen wurde, löst TrackingThread den Mutex
     * m_paintMutex.
     * @param fps
     * @details Does exactly one tick, eg. drawing one image and starting tracker once.
     */
    void tick(const double fps);

    /**
     * @brief TrackingThread::nextFrame
     * @details
     * -# Falls im ImageStream ein weiterer Frame vorhanden ist ImageStream::nextFrame(),
     * wird dieser in ImageStream automatisch als aktueller Frame gesetzt.
     *  -# Sobald ImageStream den aktuellen Frame gesetzt hat, wird TrackingAlgorithm mit einem
     * Mutex m_trackerMutex gelocked.
     *  -# Sobald TrackingAlgorithm gelocked ist und ein TrackingAlgorithm vorhanden ist, wird
     * die aktuelle FrameNumber aus ImageStream::currentFrameNumber() an den TrackingAlgorithm::setCurrentFrameNumber()
     * übergeben.
     *  -# Soblad die FrameNumber an den TrackingAlgorithm übergeben wurde, wird der Mutex m_trackerMutex gelöst.
     * -# Falls im ImageStram kein weiterer Frame vorhanden ist, verlässt TrackingAlgorithm die Zustände
     *  -# playing m_playing und
     *  -# playOnce m_playOnce.
     */
    void nextFrame();


    /**
     * @brief @req{1547} With the function run() TrackingThread has the ability of creating a sub thread
     * of the main application thread.
     */
    virtual void run() override;

  public Q_SLOTS:

    /**
    * @brief This SLOT provides a sender with the ability to send a certain frameNumber,
    * that will be tracked in TrackingAlgorithm
    * @param frameNumber specifies the current frame number.
    *
    * @satisfy{@req{1532}}
    */
    void setFrameNumber(size_t frameNumber);

    /**
    * @brief This fuction returns the current frame number.
    * This function provides the ability to request the current frame number.
    *
    * Diese Funktion bietet die Möglichkeit, die aktuelle frameNumber abzufragen.<BR>
    * TrackingThread::getFrameNumber()
    *
    * @todo It works but: This should not be a public slot but a public fuction.
    *
    * @satisfy{@req{1533}}
    */
    size_t getFrameNumber() const;

    /**
    * @brief This SLOT provides a sender with the ability to submit a new speed (fps)
    * for a Video, that will be passed over to TrackingAlgorithm.
    * @param fps The speed in which the video will be played.
    *
    * @satisfy{@req{1534}}
    */
    void setFps(double fps);

    /**
    * @brief This SLOT provides a sender with the ability to run the tracking TrackingAlgorithm
    * with maximum speed possible. It also provides the ability to run the tracking not with
    * the maxiumm speed.
    *
    * @note What happens if set MaxSpeed is set to false. I think such a function shoult only be a
    * trigger to get to the maximum speed. The undoing of this should be done somewhere else.
    *
    * @satisfy{@req{1535}}
    */
    void setMaxSpeed(bool enabled);


    /**
     * @brief @req{1537} TrackingThread provides a sender with the ability to automatically set a new TrackingAlgorithm.
     * <BR> @req{1537}-1 If TrackingThread::setTrackingAlgorithm() receives a TrackingAlgorithm and if there was allready a tracker TrackingAlgorithm in the TrackingThread,
     * this function is able to disconnect all SIGNALs and SLOTs from the old tracker TrackingAlgorithm.
     * <BR> @req{1537}-2 If TrackingThread::setTrackingAlgorithm() receives a TrackingAlgorithm, TrackingThread is able to connect it's
     * SLOTs to TrackingAlgorithm SIGNALs.
     * <BR> @req{1537}-3 If a TrackingAlgorithm was set, TrackingThread::setTrackingAlgorithm() is able to inform all receivers of TrackingThread::trackerSelected()
     * about the the Tracker and emitts the TrackingAlgorithm to them.
     * <BR> @req{1537}-4 If TrackingThread::setTrackingAlgorithm() receives a TrackingAlgorithm, this function uses a MutexLocker to make shure
     * beeing the only one accessing TrackingAlgorithm.
     *
     * @satisfy{@req{1537}}
     * @note The connecting should be encapsulated in a separate function.
     *          -# TrackingAlgorithm::registerViews()
     *          -# TrackingAlgorithm::update()
     *          -# TrackingAlgorithm::forceTracking()
     *          -# TrackingAlgorithm::notifyGUI()
     *          -# TrackingAlgorithm::pausePlayback()
     *
     */
    void setTrackingAlgorithm(std::shared_ptr<TrackingAlgorithm> TrackingAlgorithm);

    /**
     * @brief This SLOT provides a sender with the ability to submit one or more views TrackingAlgorithm::View.
     * @param views
     * @satisfy{@req{1538}}
     */
    void registerViewsFromTracker(const std::vector<TrackingAlgorithm::View> views);

    /**
     * @brief This SLOT provides the a sender (e.g. TrackingAlgorithm) with the ability to ask for a repaint.
     *
     * @satisfy{@req{1539}}
     */
    void requestPaintFromTracker();

    /**
     * @brief This SLOT calls the function TrackingThread::playOnce()
     *
     * @todo What is the fuction TrackingThread::requestPaintFromTracker() doing?
     * This function calles TrackingThread::playOnce().
     */
    void requestTrackFromTracker();

    /**
     * @brief This function should not be used.
     *
     * @bug The function TrackingThread::requestPauseFromTracker() should not exist like this.
     * Whit this function one can ask for Pausing but also set the TrackingThread to paly. That
     * makes no sense.
     * Solution: remove variable pause an let this function only set the state of the TrackingThread
     * to pause. Delete part with paly.
     */
    void requestPauseFromTracker(bool pause);

    /**
     * @brief This SLOT provides the a sender the ability to recieve a Message and it's MessageType
     * and pass is on to the receiver of the SIGNAL TrackingThread::notifyGUI()
     * @param m This is the message string.
     * @param type This is the type of the message MessageType
     *
     * @satisfy{@req{1540}}
     */
    void notifyGUIFromTracker(std::string m, MessageType type = MessageType::NOTIFICATION);

  Q_SIGNALS:
    /**
    * @brief Durch dieses SIGNAL hat der TrackingThread die Fähigkeit,
    * automatisch die aktuelle frameNumber mit filename und aktuellen fps an einen Empfänger zu senden.
    * @param frameNumber The current frame number.
    * @param filename The current file name
    * @param currentFps The fps of the last frame: when the fps are -1, no fps should be shown
    *
    * @satisfy{@req{1541}}
    */
    void frameCalculated(const size_t frameNumber, const std::string filename,
                         const double currentFps);

    /**
     * @brief @req{1542} Wenn ein File geöffnet wird, hat der TrackingThread durch das SIGNAL TrackingThread::fileOpened() die Fähigkeit,
     * automatisch die Attribute des Files (fileName, die Gesamtzahl der Frames und die Fps) an einen
     * Receiver zu übermitteln.
     * @param fileName Sting mit dem FilePath.
     * @param numFrame Gesamtzahl der im File enthaltenen Frames.
     * @param targetFps FPS des Files.
     *
     *
     *
     * @satisfy{@req{1542}}
     */
    void fileOpened(const std::string fileName, const size_t numFrame,
                    const double targetFps);

    /**
     * @brief @req{1543} TrackingThread ist Fähig, durch das SIGNAL TrackingThread::notifyGUI()
     * automatisch allen Receivern einen Message string mit MessageType zu überreichen.
     * @param message A string containing the message.
     * @param type The type of the message. MessageType
     *
     * @satisfy{@req{1543}}
     */
    void notifyGUI(std::string message,
                   MessageType type = MessageType::NOTIFICATION);

    /**
     * @brief @req{1544} With the SIGNAL trackerSelected(); TrackingThread has the
     * ability to send a pointer of the TrackingAlgorithm object to all receivers.
     * @param tracker A pointer to the TrackingAlgorithm object.
     */
    void trackerSelected(std::shared_ptr<TrackingAlgorithm> tracker);

    /**
     * @brief @req{1545} With the SIGNAL registerViews(), TrackingThread has the
     * ability to send one or more TrackingAlgorithm::View object to all receivers.
     * @param views Is a vector of TrackingAlgorithm::View
     */
    void registerViews(const std::vector<TrackingAlgorithm::View> views);

    /**
     * @brief @req{1546} With the SIGNAL requestPaint(), TrackingThread has the
     * ability to ask all receivers of this SIGNAL for a repaint.
     */
    void requestPaint();
};

}
}
