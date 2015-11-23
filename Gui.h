#pragma once

#include <QStringList>

#include "biotracker/BioTrackerApp.h"
//#include "biotracker/ZmqTrackingAlgorithm.h"

#include "MainWindow.h"

#include "biotracker/util/QOpenGLContextWrapper.h"

namespace BioTracker {
namespace Gui {

class Gui : public QObject {
    Q_OBJECT
  public:
    Gui();
  private:
    Core::BioTrackerApp m_biotracker;
    MainWindow m_mainWindow;

    void initConnects();
    QStringList browse(const QString &title, const QString &filter);

  private Q_SLOTS:
    void browseVideo();
    void browsePictures();
    void browseCameras();

    void loadTracker();
//    void loadZmqTracker(BioTracker::Core::Zmq::ZmqInfoFile &info);

    void loadTrackingData();
    void storeTrackingData();

    void exit();
};



}
}
