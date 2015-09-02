#include "VideoControlWidget.h"

#include <QKeySequence>
#include <QShortcut>
#include <thread>

#include "biotracker/core/TrackingThread.h"
#include "biotracker/gui/VideoView.h"


namespace BioTracker {
namespace Gui {

VideoControlWidget::VideoControlWidget(QWidget *parent,
                                       Core::BioTrackerApp &facade, VideoView *videoView)
    : QWidget(parent)
    , m_ui(parent)
    , m_bioTracker(facade)
    , m_videoView(videoView)
    , m_isPanZoomMode(false) {
    m_iconPause.addFile(QStringLiteral(":/BioTracker/resources/pause-sign.png"),
                        QSize(), QIcon::Normal, QIcon::Off);
    m_iconPlay.addFile(QStringLiteral(":/BioTracker/resources/arrow-forward1.png"),
                       QSize(), QIcon::Normal, QIcon::Off);

    initConnects();
    initShortcuts();
}

void VideoControlWidget::updateWidgets() {
    const bool validFile = m_bioTracker.getStatus() ==
                           Core::TrackingThread::TrackerStatus::Running ||
                           m_bioTracker.getStatus() == Core::TrackingThread::TrackerStatus::Paused;

    const bool hasNext = m_bioTracker.getCurrentFrameNumber() <
                         m_bioTracker.getNumFrames();
    const bool hasPrev = m_bioTracker.getCurrentFrameNumber() > 0;

    m_ui.button_nextFrame->setEnabled(validFile && hasNext);
    m_ui.button_previousFrame->setEnabled(validFile && hasPrev);

    switch (m_bioTracker.getStatus()) {
    case Core::TrackingThread::TrackerStatus::Invalid:
    case Core::TrackingThread::TrackerStatus::NothingLoaded:
        m_ui.button_playPause->setIcon(m_iconPlay);
        m_ui.button_playPause->setEnabled(false);
        break;
    case Core::TrackingThread::TrackerStatus::Running:
        m_ui.button_playPause->setIcon(m_iconPause);
        m_ui.button_playPause->setEnabled(true);
        break;
    case Core::TrackingThread::TrackerStatus::Paused:
        m_ui.button_playPause->setIcon(m_iconPlay);
        m_ui.button_playPause->setEnabled(true);
        break;
    default:
        assert(false);
    }
}

void VideoControlWidget::initShortcuts() {
    const QString shortcutPanKey = QString::fromStdString(
                                       m_bioTracker.getSettings().getValueOrDefault<std::string>
                                       (GUIPARAM::SHORTCUT_ZOOM,"Z"));
    const QShortcut *shortcutPan = new QShortcut(QKeySequence(shortcutPanKey),
            this);
    QObject::connect(shortcutPan, &QShortcut::activated, m_ui.button_panZoom,
                     &QPushButton::click);

    const QString shortcutPlayKey = QString::fromStdString(
                                        m_bioTracker.getSettings().getValueOrDefault<std::string>
                                        (GUIPARAM::SHORTCUT_PLAY,"Space"));
    const QShortcut *shortcutPlay = new QShortcut(QKeySequence(shortcutPlayKey),
            this);
    QObject::connect(shortcutPlay, &QShortcut::activated, m_ui.button_playPause,
                     &QPushButton::click);

    const QString shortcutNextKey = QString::fromStdString(
                                        m_bioTracker.getSettings().getValueOrDefault<std::string>
                                        (GUIPARAM::SHORTCUT_NEXT,"Right"));
    const QShortcut *shortcutNext = new QShortcut(QKeySequence(shortcutNextKey),
            this);
    QObject::connect(shortcutNext, &QShortcut::activated, m_ui.button_nextFrame,
                     &QPushButton::click);

    const QString shortcutPrevKey = QString::fromStdString(
                                        m_bioTracker.getSettings().getValueOrDefault<std::string>
                                        (GUIPARAM::SHORTCUT_PREV,"Left"));
    const QShortcut *shortcutPrev = new QShortcut(QKeySequence(shortcutPrevKey),
            this);
    QObject::connect(shortcutPrev, &QShortcut::activated, m_ui.button_previousFrame,
                     &QPushButton::click);
}

void VideoControlWidget::initConnects() {
    QObject::connect(m_ui.button_playPause, &QPushButton::clicked, this,
                     &VideoControlWidget::playPause);
    QObject::connect(m_ui.button_nextFrame, &QPushButton::clicked, this,
                     &VideoControlWidget::nextFrame);
    QObject::connect(m_ui.button_previousFrame, &QPushButton::clicked, this,
                     &VideoControlWidget::previousFrame);
    QObject::connect(m_ui.frame_num_edit, &QLineEdit::returnPressed, this,
                     &VideoControlWidget::changeCurrentFrameByEdit);
    QObject::connect(m_ui.button_screenshot, &QPushButton::clicked, this,
                     &VideoControlWidget::takeScreenshot);
    QObject::connect(m_ui.button_panZoom, &QPushButton::clicked, this,
                     &VideoControlWidget::switchPanZoomMode);
}

void VideoControlWidget::playPause() {
    switch (m_bioTracker.getStatus()) {
    case Core::TrackingThread::TrackerStatus::Paused:
        m_bioTracker.play();
        break;
    case Core::TrackingThread::TrackerStatus::Running:
        m_bioTracker.pause();
        break;
    default:
        assert(false);
    }

    updateWidgets();
}

void VideoControlWidget::setFrame(const size_t frame) {
    assert(frame < m_bioTracker.getNumFrames());
    m_bioTracker.setFrame(frame);

    updateWidgets();
}

void VideoControlWidget::nextFrame() {
    assert(m_bioTracker.getCurrentFrameNumber() < m_bioTracker.getNumFrames());
    m_bioTracker.setFrame(m_bioTracker.getCurrentFrameNumber() + 1);

    updateWidgets();
}

void VideoControlWidget::previousFrame() {
    assert(m_bioTracker.getCurrentFrameNumber() > 0);
    m_bioTracker.setFrame(m_bioTracker.getCurrentFrameNumber() - 1);

    updateWidgets();
}

void VideoControlWidget::changeCurrentFrameByEdit() {
    setFrame(m_ui.sld_video->value());
}

void VideoControlWidget::takeScreenshot() {
    /*
    QString filename = "";
    const std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
    const std::time_t t = std::chrono::system_clock::to_time_t(p);
    std::string dateTime = std::ctime(&t);
    //ctime adds a newline to the string due to historical reasons
    dateTime = dateTime.substr(0, dateTime.size() - 1);
    filename.append("/screenshot_").append(QString::fromStdString(dateTime)).append(".png");
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save File"), "/home/"+filename , tr("Images (*.png)"));
    ui.videoView->takeScreenshot(filepath);
    */
}

void VideoControlWidget::switchPanZoomMode() {
    switch (m_videoView->getMode()) {
    case VideoView::Mode::INTERACTION:
        m_videoView->setMode(VideoView::Mode::PANZOOM);
        break;
    case VideoView::Mode::PANZOOM:
        m_videoView->setMode(VideoView::Mode::INTERACTION);
        break;
    default:
        assert(false);
        break;
    }
}

void VideoControlWidget::frameCalculated(const size_t frameNumber,
        const std::string filename, const double currentFps) {

    std::cout << "frame_calc:" << filename << " fps:" << currentFps << "\n";

    //m_videoView->update(); // does not work somehow -.-
    m_videoView->resize(m_videoView->width()+1, m_videoView->height());
    m_videoView->resize(m_videoView->width()-1, m_videoView->height());

    m_ui.lbl_frame->setText(QString::number(frameNumber));
    m_ui.fps_label->setText(QString::number(currentFps));
}

} // Gui
} // BioTracker
