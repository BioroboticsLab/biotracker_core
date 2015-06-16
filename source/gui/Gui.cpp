#include "Gui.h"

#include <vector>

#include <boost/filesystem.hpp>

#include <QAction>
#include <QFileDialog>


#include <QShortcut>
#include <QKeySequence>


namespace BioTracker {
namespace Gui {

Gui::Gui()
    : m_isPanZoomMode(false)
{
    initConnects();

    m_mainWindow.show();
}

void Gui::initConnects()
{
    //Start Up
    //QObject::connect(this, SIGNAL(window_loaded()), this, SLOT(startUp()), Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));

    //File -> Open Video
    QObject::connect(m_mainWindow.getUi().actionOpen_Video, &QAction::triggered, this, &Gui::browseVideo);
    QObject::connect(m_mainWindow.getUi().actionOpen_Picture, &QAction::triggered, this, &Gui::browsePictures);

    /*       _______________________
        *   |                       |
        *   | connect shortcut keys |
        *   |_______________________| */
    // Pan&Zoom
    QShortcut *shortcutPan = new QShortcut(QKeySequence
                                           (QString::fromStdString(m_facade.getSettings().getValueOrDefault<std::string>(GUIPARAM::SHORTCUT_ZOOM,"Z"))), &m_mainWindow);
    QObject::connect(shortcutPan, &QShortcut::activated, m_mainWindow.getUi().button_panZoom, &QPushButton::click);

    // Play*Pause
    QShortcut *shortcutPlay = new QShortcut(QKeySequence
                                            (QString::fromStdString(m_facade.getSettings().getValueOrDefault<std::string>(GUIPARAM::SHORTCUT_PLAY,"Space"))), &m_mainWindow);
    QObject::connect(shortcutPlay, &QShortcut::activated, m_mainWindow.getUi().button_playPause, &QPushButton::click);

    // Next Frame
    QShortcut *shortcutNext = new QShortcut(QKeySequence
                                            (QString::fromStdString(m_facade.getSettings().getValueOrDefault<std::string>(GUIPARAM::SHORTCUT_NEXT,"Right"))), &m_mainWindow);
    QObject::connect(shortcutNext, &QShortcut::activated, m_mainWindow.getUi().button_nextFrame, &QPushButton::click);
    // Previous Frame
    QShortcut *shortcutPrev = new QShortcut(QKeySequence
                                            (QString::fromStdString(m_facade.getSettings().getValueOrDefault<std::string>(GUIPARAM::SHORTCUT_PREV,"Left"))), &m_mainWindow);
    QObject::connect(shortcutPrev, &QShortcut::activated, m_mainWindow.getUi().button_previousFrame, &QPushButton::click);
}

void Gui::browseVideo()
{
    static const QString videoFilter("Video files (*.avi, *.wmv, *.mp4, *.mkv)");
    std::vector<boost::filesystem::path> files;

    const QString filename = QFileDialog::getOpenFileName(&m_mainWindow, "Open video", "", videoFilter);

    if (!filename.isEmpty()) {
        m_facade.openVideo(boost::filesystem::path(filename.toStdString()));
    }
}

void Gui::browsePictures()
{
    static const QString imageFilter(
        "image files (*.png *.jpg *.jpeg *.gif *.bmp *.jpe *.ppm *.tiff *.tif *.sr *.ras *.pbm *.pgm *.jp2 *.dib)");

    std::vector<boost::filesystem::path> files;
    for (QString const& path : QFileDialog::getOpenFileNames(&m_mainWindow, "Open image files", "", imageFilter)) {
        files.push_back(boost::filesystem::path(path.toStdString()));
    }

    if (!files.empty()) {
        m_facade.openImages(files);
    }
}

void Gui::switchPanZoomMode()
{
    m_isPanZoomMode = !m_isPanZoomMode;
    m_mainWindow.getVideoView()->setPanZoomMode(m_isPanZoomMode);
}

} // Gui
} // BioTracker
