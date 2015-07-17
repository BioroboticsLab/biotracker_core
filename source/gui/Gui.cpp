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
    : m_mainWindow(m_facade)
{
    BioTrackerApp(&m_mainWindow.getVideoView());
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
	QObject::connect(m_mainWindow.getUi().actionOpen_Camera, &QAction::triggered, this, &Gui::browseCameras);

    QObject::connect(m_mainWindow.getUi().actionLoad_Tracker, &QAction::triggered, this, &Gui::loadTracker);

    QObject::connect(m_mainWindow.getUi().actionLoad_tracking_data, &QAction::triggered, this, &Gui::loadTrackingData);
    QObject::connect(m_mainWindow.getUi().actionSave_tracking_data, &QAction::triggered, this, &Gui::storeTrackingData);

    QObject::connect(m_mainWindow.getUi().actionQuit, &QAction::triggered, this, &Gui::exit);

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

        // TODO: remove
        //m_mainWindow.getVideoView().setImage(cv::imread(files[0].string()));
        m_mainWindow.getVideoControl().updateWidgets();
    }
}

void Gui::browseCameras()
{
	OpenCameraDialog& cameraDialog = m_mainWindow.getOpenCameraDialog();
	cameraDialog.show();
}

void Gui::loadTracker()
{
    static const QString trackerFilter("BioTracker algorithms (*.tracker)");

    const QString path = QFileDialog::getOpenFileName(&m_mainWindow, "Load Tracker", "", trackerFilter);
    if (!path.isEmpty()) {
        boost::filesystem::path libraryPath(path.toStdString());
        m_facade.getRegistry().loadTrackerLibrary(libraryPath);
    }
}

void Gui::loadTrackingData()
{

}

void Gui::storeTrackingData()
{

}

void Gui::exit()
{

}

} // Gui
} // BioTracker
