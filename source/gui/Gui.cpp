#include "Gui.h"

#include <QAction>


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
    QObject::connect(m_mainWindow.getUi().actionOpen_Video, &QAction::triggered, this, Gui::browseVideo());
    QObject::connect(m_mainWindow.getUi().actionOpen_Picture, &QAction::triggered, this, Gui::browsePictures());
}

void Gui::browseVideo()
{

}

void Gui::browsePictures()
{

}

}
}
