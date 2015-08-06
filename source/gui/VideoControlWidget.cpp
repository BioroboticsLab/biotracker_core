#include "VideoControlWidget.h"

#include <QKeySequence>
#include <QShortcut>

#include "source/core/TrackingThread.h"
#include "source/gui/VideoView.h"

namespace BioTracker {
namespace Gui {

VideoControlWidget::VideoControlWidget(QWidget *parent, Core::BioTrackerApp& facade, VideoView *videoView)
    : QWidget(parent)
    , m_ui(parent)
    , m_facade(facade)
    , m_videoView(videoView)
    , m_isPanZoomMode(false)
{
    m_iconPause.addFile(QStringLiteral(":/BioTracker/resources/pause-sign.png"), QSize(), QIcon::Normal, QIcon::Off);
    m_iconPlay.addFile(QStringLiteral(":/BioTracker/resources/arrow-forward1.png"), QSize(), QIcon::Normal, QIcon::Off);

    initConnects();
    initShortcuts();
}

void VideoControlWidget::updateWidgets()
{
    const bool validFile = m_facade.getStatus() == Core::TrackingThread::TrackerStatus::Running ||
                           m_facade.getStatus() == Core::TrackingThread::TrackerStatus::Paused;

    const bool hasNext = m_facade.getCurrentFrameNumber() < m_facade.getNumFrames();
    const bool hasPrev = m_facade.getCurrentFrameNumber() > 0;

    m_ui.button_nextFrame->setEnabled(validFile && hasNext);
    m_ui.button_previousFrame->setEnabled(validFile && hasPrev);

    switch (m_facade.getStatus()) {
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

void VideoControlWidget::initShortcuts()
{
    const QString shortcutPanKey = QString::fromStdString(m_facade.getSettings().getValueOrDefault<std::string>(GUIPARAM::SHORTCUT_ZOOM,"Z"));
    const QShortcut *shortcutPan = new QShortcut(QKeySequence(shortcutPanKey), this);
    QObject::connect(shortcutPan, &QShortcut::activated, m_ui.button_panZoom, &QPushButton::click);

    const QString shortcutPlayKey = QString::fromStdString(m_facade.getSettings().getValueOrDefault<std::string>(GUIPARAM::SHORTCUT_PLAY,"Space"));
    const QShortcut *shortcutPlay = new QShortcut(QKeySequence(shortcutPlayKey), this);
    QObject::connect(shortcutPlay, &QShortcut::activated, m_ui.button_playPause, &QPushButton::click);

    const QString shortcutNextKey = QString::fromStdString(m_facade.getSettings().getValueOrDefault<std::string>(GUIPARAM::SHORTCUT_NEXT,"Right"));
    const QShortcut *shortcutNext = new QShortcut(QKeySequence(shortcutNextKey), this);
    QObject::connect(shortcutNext, &QShortcut::activated, m_ui.button_nextFrame, &QPushButton::click);

    const QString shortcutPrevKey = QString::fromStdString(m_facade.getSettings().getValueOrDefault<std::string>(GUIPARAM::SHORTCUT_PREV,"Left"));
    const QShortcut *shortcutPrev = new QShortcut(QKeySequence(shortcutPrevKey), this);
    QObject::connect(shortcutPrev, &QShortcut::activated, m_ui.button_previousFrame, &QPushButton::click);
}

void VideoControlWidget::initConnects()
{
    QObject::connect(m_ui.button_playPause, &QPushButton::clicked, this, &VideoControlWidget::playPause);
    QObject::connect(m_ui.button_nextFrame, &QPushButton::clicked, this, &VideoControlWidget::nextFrame);
    QObject::connect(m_ui.button_previousFrame, &QPushButton::clicked, this, &VideoControlWidget::previousFrame);
    QObject::connect(m_ui.frame_num_edit, &QLineEdit::returnPressed, this, &VideoControlWidget::changeCurrentFrameByEdit);
    QObject::connect(m_ui.button_screenshot, &QPushButton::clicked, this, &VideoControlWidget::takeScreenshot);
    QObject::connect(m_ui.button_panZoom, &QPushButton::clicked, this, &VideoControlWidget::switchPanZoomMode);
}

void VideoControlWidget::playPause()
{
    switch (m_facade.getStatus()) {
    case Core::TrackingThread::TrackerStatus::Paused:
        m_facade.play();
        break;
    case Core::TrackingThread::TrackerStatus::Running:
        m_facade.pause();
        break;
    default:
        assert(false);
    }

    updateWidgets();
}

void VideoControlWidget::setFrame(const int frame)
{
    assert(frame >= 0);
    assert(static_cast<size_t>(frame) < m_facade.getNumFrames());
    m_facade.setFrame(frame);

    updateWidgets();
}

void VideoControlWidget::nextFrame()
{
    assert(m_facade.getCurrentFrameNumber() < m_facade.getNumFrames());
    m_facade.setFrame(m_facade.getCurrentFrameNumber() + 1);

    updateWidgets();
}

void VideoControlWidget::previousFrame()
{
    assert(m_facade.getCurrentFrameNumber() > 0);
    m_facade.setFrame(m_facade.getCurrentFrameNumber() - 1);

    updateWidgets();
}

void VideoControlWidget::changeCurrentFrameByEdit()
{
    setFrame(m_ui.sld_video->value());
}

void VideoControlWidget::takeScreenshot()
{
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

void VideoControlWidget::switchPanZoomMode()
{
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

}
}
