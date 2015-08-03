#pragma once

#include <QStringList>

#include "source/core/BioTrackerApp.h"
#include "source/core/Facade.h"

#include "source/gui/MainWindow.h"

namespace BioTracker {
namespace Gui {

class Gui : public Core::BioTrackerApp {
    Q_OBJECT
public:
    Gui();

private:
    MainWindow m_mainWindow;
    QOpenGLContext m_trackingThreadContext;
    void initConnects();
    QStringList browse(const QString &title, const QString &filter);

private slots:
    void browseVideo();
    void browsePictures();
    void browseCameras();

    void loadTracker();

    void loadTrackingData();
    void storeTrackingData();

    void exit();
};



}
}
