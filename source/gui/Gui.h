#pragma once

#include "source/core/BioTrackerApp.h"
#include "source/gui/MainWindow.h"

namespace BioTracker {
namespace Gui {

class Gui : public Core::BioTrackerApp {
    Q_OBJECT
public:
    Gui();

private:
    MainWindow m_mainWindow;

    void initConnects();

private slots:
    void browseVideo();
    void browsePictures();
};



}
}
