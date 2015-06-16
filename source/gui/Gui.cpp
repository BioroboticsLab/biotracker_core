#include "Gui.h"

#include <vector>

#include <boost/filesystem.hpp>

#include <QAction>
#include <QFileDialog>


namespace BioTracker {
namespace Gui {

Gui::Gui()
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

} // Gui
} // BioTracker
