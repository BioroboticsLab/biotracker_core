#pragma once

#include <QMainWindow>

#include "source/gui/ui_MainWindow.h"
#include "source/gui/VideoView.h"

namespace BioTracker {
namespace Gui {

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();

    const Ui::MainWindowUi& getUi() const { return m_ui; }
    VideoView *getVideoView() const { return m_ui.videoView; }

private:
    Ui::MainWindowUi m_ui;
};



}
}
