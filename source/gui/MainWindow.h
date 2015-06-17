#pragma once

#include <QMainWindow>

#include "source/core/Facade.h"
#include "source/gui/ui_MainWindow.h"
#include "source/gui/AlgorithmSelectionWidget.h"
#include "source/gui/NotificationWidget.h"
#include "source/gui/VideoControlWidget.h"

namespace BioTracker {
namespace Gui {

class VideoView;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(Core::Facade &facade);

    const Ui::MainWindow& getUi() const { return m_ui; }

    // TODO: refactor
    VideoView *getVideoView() const { return m_ui.videoView; }

    const AlgorithmSelectionWidget& getAlgorithmSelection() const { return m_algorithmSelection; }
    const NotificationWidget& getNotification() const { return m_notification; }
    const VideoControlWidget& getVideoControl() const { return m_videoControl; }

private:
    Ui::MainWindow m_ui;
    AlgorithmSelectionWidget m_algorithmSelection;
    NotificationWidget m_notification;
    VideoControlWidget m_videoControl;
};



}
}
