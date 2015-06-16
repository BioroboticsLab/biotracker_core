#pragma once

#include <QMainWindow>

#include "source/core/Facade.h"
#include "source/gui/ui_MainWindow.h"
#include "source/gui/VideoControlWidget.h"

namespace BioTracker {
namespace Gui {

class VideoView;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(Core::Facade &facade);

    const Ui::MainWindow& getUi() const { return m_ui; }
    VideoView *getVideoView() const { return m_ui.videoView; }
    const VideoControlWidget& getVideoControl() const { return m_videoControl; }

private:
    Ui::MainWindow m_ui;
    VideoControlWidget m_videoControl;
};



}
}
