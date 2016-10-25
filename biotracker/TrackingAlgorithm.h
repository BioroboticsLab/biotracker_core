#pragma once

#include <vector>
#include <memory>
#include <set>

#include <boost/optional.hpp>

#include <opencv2/opencv.hpp>

#include <QWidget>
#include <QPointer>
#include <QMouseEvent>

#include "util/platform.h"
#include "settings/Messages.h"
#include "settings/ParamNames.h"
#include "serialization/TrackedObject.h"


namespace BioTracker {
namespace Core {

/**
 *@page TARequirements Requirements TrackingAlgorithm [13xx and 14xx]
 *
 * -# @anchor REQ_1311 REQ-1311 <BR>
 * @copybrief TrackingAlgorithm::TrackingAlgorithm()
 * <BR> - TrackingAlgorithm::TrackingAlgorithm()
 *
 * -# @anchor REQ_1312 REQ-1312 <BR>
 * @copybrief TrackingAlgorithm::View
 * <BR> - TrackingAlgorithm::View und siehe auch
 * <BR> - TrackingAlgorithm::OriginalView
 *
 * -# @anchor REQ_1313 REQ-1313 <BR>
 * @copybrief TrackingAlgorithm::attemptTracking()
 * <BR> - TrackingAlgorithm::attemptTracking()
 *
 * -# @anchor REQ_1314 REQ-1314 <BR>
 * @copybrief TrackingAlgorithm::paint()
 * <BR> - TrackingAlgorithm::paint()
 *
 * -# @anchor REQ_1315 REQ-1315 <BR>
 * @copybrief TrackingAlgorithm::paintOverlay()
 * <BR> - TrackingAlgorithm::paintOverlay()
 *
 * -# @anchor REQ_1316 REQ-1316 <BR>
 * @copybrief TrackingAlgorithm::getToolsWidget()
 * <BR> - TrackingAlgorithm::getToolsWidget()
 *
 * -# @anchor REQ_1317 REQ-1317 <BR>
 * @copybrief TrackingAlgorithm::grabbedKeys()
 * <BR> - TrackingAlgorithm::grabbedKeys()
 *
 * -# @anchor REQ_1318 REQ-1318 <BR>
 * @copybrief TrackingAlgorithm::prepareSave()
 * <BR> - TrackingAlgorithm::prepareSave()
 *
 * -# @anchor REQ_1319 REQ-1319 <BR>
 * @copybrief TrackingAlgorithm::postLoad()
 * <BR> - TrackingAlgorithm::postLoad()
 *
 * -# @anchor REQ_1320 REQ-1320 <BR>
 * @copybrief TrackingAlgorithm::postConnect()
 * <BR> - TrackingAlgorithm::postConnect()
 *
 * -# @anchor REQ_1321 REQ-1321 <BR>
 * @copybrief TrackingAlgorithm::inputChanged()
 * <BR> - TrackingAlgorithm::inputChanged()
 *
 * -# @anchor REQ_1322 REQ-1322 <BR>
 * @copybrief TrackingAlgorithm::onFileChanged()
 * <BR> - TrackingAlgorithm::onFileChanged()
 *
 * @section TAsreq Satisfied Requirements 13xx
 *
 *
 * @section TAureq Unsatisfied Requirements 14xx
 *
 *
 */

class Settings;

namespace Algorithm {
typedef uint8_t Type;
}

class ProxyMat {
  public:
    ProxyMat(cv::Mat const &mat)
        : m_originalMat(mat) {
    }

    ProxyMat(const ProxyMat &) = delete;
    ProxyMat &operator=(const ProxyMat &) = delete;

    cv::Mat &getMat() {
        if (!isModified()) {
            m_modifiedMat = m_originalMat.clone();
        }
        return m_modifiedMat.get();
    }

    void setMat(cv::Mat mat) {
        m_modifiedMat = mat;
    }

    bool isModified() const {
        return m_modifiedMat.is_initialized();
    }

  private:
    cv::Mat const &m_originalMat;
    boost::optional<cv::Mat> m_modifiedMat;
};

class BIOTRACKER_DLLEXPORT TrackingAlgorithm : public QObject {
    Q_OBJECT

  public:

    /**
     * @brief @req{1311} Wenn TrackingAlgorithm initialisiert wird,
     * <br> - hat er die Fähigkeit ein Objekt vom Typ Settings zu speichern.
     * <br> - ist sein Status doTracking.
     * <br> - ist die aktuelle FrameNumber 0.
     * <br> - ist das aktuelle ZoomLevel 0.
     * <br> - ist der VideoMode GuiParam::VideoMode::Init
     * <br> - erstellt er automatisch ein QWidget m_tools für seine spezifischen GUIControlls.
     * @param settings
     */
    TrackingAlgorithm(Settings &settings);
    virtual ~TrackingAlgorithm() override;

    /**
     * @brief @req{1312} TrackingAlgorithm beinhalten ein Struct mit dem Namen View. Dies wird
     * beinhalten den String "Original", der zur GUI übertragen wird und auf seinem Weg nichts macht.
     */
    struct BIOTRACKER_DLLEXPORT View {
        std::string name;
    };

    /**
     * @brief @req{1312} OriginalView: Hier ist fest der String "Original" codiert. Warum?
     */
    static const View OriginalView;

    /**
     * @brief @req{1313} TrackingAlgorithm versucht die Methode track TrackingAlgorithm::track()
     * aufzurufen. Dies geschieht nur, sein Status TrackingAlgorithm::isTrackingActivated() ist.
     * Wenn nicht, bricht die Funktion ab. (nicht so gut implementiert, da es mehrere returns gibt.
     * Wenn track() nicht erfolgreich beendet wird, gibt TrackingAlgorithm::attemptTracking()
     * dennoch true zurück!!! ACHTUNG !!! das passt nicht!
     * @param frameNbr
     * @param frame
     * @return True, when successfully tracked, otherwise False <-- Diese Aussage ist falsch!
     */
    bool attemptTracking(size_t frameNbr, const cv::Mat &frame) {
        if (isTrackingActivated()) {
            track(frameNbr, frame);
            return true;
        } else {
            return false;
        }
    }

    /**
    * @brief @req{1314} TrackingAlgorithm bietet dem Programmierer die Möglichkeit
    * mit der Funktion TrackingAlgorithm::paint() direkt die aktuelle ProxyMat zu manipulieren.
    * <br> --> dies trifft nicht zu! "paint will be called by "VideoViews" paintEvent method
    * so any picture manipulation stuff goes in here.
    * QPainter paints stuff onto "VideoViews" current picture
    * without touching it"
    * <br> Wozu dient die View const & = OriginalView ?
    */
    virtual void paint(size_t, ProxyMat &, View const & = OriginalView) {}

    /**
     * @brief @req{1315} TrackingAlgorithm zeichnet auf einen QPainter, der vom
     * VideoView die BioTrackerApp::paint(), den TrackingThread::paint() zum
     * TrackingAlgorithm::paintOverlay()
     * durchgereicht wird. Dies verstößt gegen MVC (Entwurfsmuster S.525). Die View darf nicht
     * seine Daten in das Model geben.
     * <br> Wozu dient die View const & = OriginalView ?
     */
    virtual void paintOverlay(size_t, QPainter *, View const & = OriginalView) {}

    /**
    * @brief @req{1316} TrackingAlgorithm bietet dem Programmierer die Möglichkeit ein
    * QWidget zu spezielle für die Bedürfnisse des Trackers zu gestalten.
    * <br> - Außerdem bietet TrackingAlgorithm die Möglichkeit dieses QWidget
    * abzurufen (z.B. von der GUI)
    */
    virtual QPointer<QWidget> getToolsWidget();

    /**
     * @brief @req{1317} TrackingAlgorithm bietet dem Programmierer die Möglichkeit
     * ein Set mit KeyEvents zu definieren. !!! Dies wird nirgendwo benutzt !!!
     * Siehe dazu auch TrackingAlgorithm::keyPressEvent(). In den Trackern
     * BeeDance und LucasKanade wird diese Funktionalität in der Methode keyPressEvent();
     * abgebildet. !! Es ist wohl nicht klar, welchen
     * UseCase diese Methode abdeckt !!
     * @return const reference to the set of keys
     */
    virtual std::set<Qt::Key> const &grabbedKeys() const;

    /**
     * @brief @req{1318} prepareSave(); is called once before the serialization of
     * m_trackedObjects. It should store or discard all temporary values
     * that are related to tracked Objects.
     * <br> wurde zwar deklariert wird aber !! nirgendwo aufgerufen !! !! Es ist wohl nicht klar, welchen
     * UseCase diese Methode abdeckt !!
     */
    virtual void prepareSave() {}

    /**
     * @brief @req{1319} postLoad() is called once after the tracked objects are
     * loaded from serialized data. It should do any postprocessing required.
     * <br> wurde zwar deklariert wird aber !! nirgendwo aufgerufen !! !! Es ist wohl nicht klar, welchen
     * UseCase diese Methode abdeckt !!
     */
    virtual void postLoad() {}

    /**
     * @brief @req{1320} postConnect() is called once after the tracking algorithm has
     * been initialized and the signals have been connected. It can be used
     * to emit signals that only need to be emitted during object initialization.
     * <br> wurde zwar deklariert wird aber !! nirgendwo aufgerufen !! !! Es ist wohl nicht klar, welchen
     * UseCase diese Methode abdeckt !!
     */
    virtual void postConnect() {}

    /**
     * @brief @req{1321} inputChanged
     * gets called when the input system changes (e.g. a new video is loaded)
     * When a set of images is loaded, this funtion is only triggered once
     * at the first image that is loaded.
     * <br> inputChanged(); wird in TrackingThread mehrfach (4x) aufgerufen. Diese Methode
     * wurde aber nur von LucasKanade implementiert. !! Es ist wohl nicht klar, welchen
     * UseCase diese Methode abdeckt !!
     */
    virtual void inputChanged() {}

    /**
     * @brief @req{1322} onFileChanged();
     * This slot is triggered once when a video is loaded, or each time
     * a new picture from a set im images is loaded. This slot is never
     * triggered when the camera is selected.
     * <br> wird in TrackingThread 6x aufgerufen. Sie wird jedoch in keinem Tracker
     * benutzt. !! Es ist wohl nicht klar, welchen
     * UseCase diese Methode abdeckt !!
     * @param filename
     */
    virtual void onFileChanged(std::string) {}


    void loadObjects(std::vector<TrackedObject> const &objects);
    void loadObjects(std::vector<TrackedObject> &&objects);
    std::vector<TrackedObject> const &getObjects();

    void setTracking(bool shouldTrack) {
        m_doTracking = shouldTrack;
    }

    bool isTrackingActivated() const {
        return m_doTracking;
    }

    boost::optional<Algorithm::Type> getType() const {
        return m_type;
    }
    void setType(Algorithm::Type type) {
        m_type = type;
    }

    int getCurrentFrameNumber() const {
        return m_currentFrameNumber;
    }

    int getMaxFrameNumber() const {
        return m_maxFrameNumber;
    }

    float getCurrentZoomLevel() const {
        return m_currentZoomLevel;
    }

  public Q_SLOTS:
    /**
    * receive Signal to set current frame number
    */
    void setCurrentFrameNumber(int frameNumber) {
        m_currentFrameNumber = frameNumber;
    }

    /**
    * receive Signal to set maximum frame number
    */
    void setmaxFrameNumber(int maxFrameNumber) {
        m_maxFrameNumber = maxFrameNumber;
    }

    /**
    * receive current zoom level from VideoView
    */
    void setZoomLevel(float zLevel) {
        m_currentZoomLevel = zLevel;
    }

    /**
    * receive current image from TrackingThread
    */
    void setCurrentImage(cv::Mat img) {
        m_currentImage = img;
    }

    /**
    * receive current video mode from gui
    */
    void setCurrentVideoMode(GuiParam::VideoMode videoMode) {
        m_videoMode = videoMode;
    }

    void setVideoFps(const size_t fps) {
        m_videoFps = fps;
    }

  Q_SIGNALS:
    /**
    * send a message to the GUI.
    */
    void notifyGUI(std::string message, MessageType type = MessageType::NOTIFICATION);

    /**
    * send signal to VideoView and update display
    */
    void update();

    /**
     * @bug Signals are automatically generated by the moc and
     * must not be implemented in the .cpp file. They can never have
     * return types (i.e. use void). http://doc.qt.io/qt-4.8/signalsandslots.html
     */
    cv::Mat requestCurrentScreen();

    void forceTracking();

    void registerViews(const std::vector<View> views);

    /**
     * start/pause playback
     */
    void pausePlayback(bool paused);

    /**
     * seek in framesequence
     * @param framenumber where you want to jump to
     */
    void jumpToFrame(int framenumber);

  protected:
    Settings &m_settings;
    std::vector<TrackedObject> m_trackedObjects;

    GuiParam::VideoMode getVideoMode() {
        return m_videoMode;
    }

    /**
    * This function tracks the provided object list within the provided frame.
    */
    virtual void track(size_t frameNumber, const cv::Mat &frame) = 0;

    bool event(QEvent *event) override;

    /**
     * @return either a copy of the current frame image, wrapped in a
     * boost::optional, or a unintialized boost::optional, if there is no
     * frame yet
     */
    boost::optional<cv::Mat> getCurrentImageCopy() const;

    /**
    * will receive QMouseEvent as soon
    * as mouse is getting moved in video view
    */
    virtual void mouseMoveEvent(QMouseEvent * /* e */) {}

    /**
    * will receive QMouseEvent as soon
    * as any mouse button is pressed in video view
    */
    virtual void mousePressEvent(QMouseEvent * /* e */) {}

    /**
    * will receive QMouseEvent as soon
    * as any mouse button is released in video view
    */
    virtual void mouseReleaseEvent(QMouseEvent * /* e */) {}

    /**
    * will receive QMouseEvent as soond
    * as mouse wheel is activated in video view
    */
    virtual void mouseWheelEvent(QWheelEvent * /* e */) {}


    /**
    *
    * will receive QKeyEvent as soon
    * as any keyboard key from 'grabbedKeys()' is pressed in video view
    */
    virtual void keyPressEvent(QKeyEvent * /* e */) {}

  protected:
    size_t m_videoFps = -1;

  private:
    bool m_doTracking; // determines if the tracker should track or only paint
    int m_currentFrameNumber;
    int m_maxFrameNumber;
    float m_currentZoomLevel;
    boost::optional<Algorithm::Type> m_type;
    boost::optional<cv::Mat> m_currentImage;
    GuiParam::VideoMode m_videoMode;
    QPointer<QWidget> m_tools;
};

}
}
