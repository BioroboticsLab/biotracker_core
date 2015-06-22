#pragma once

#include <QMainWindow>

#include "source/core/Facade.h"
#include "source/gui/ui_MainWindow.h"
#include "source/gui/AlgorithmSelectionWidget.h"
#include "source/gui/NotificationWidget.h"
#include "source/gui/VideoControlWidget.h"
#include "source/gui/VideoView.h"
#include "source/util/QtRaiiWrapper.hpp"
#include "source/util/QOpenGLContextWrapper.h"

namespace BioTracker {
namespace Gui {

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    typedef Util::QtRaiiWrapper<Ui::MainWindow, QMainWindow> MainWindowUi;

    MainWindow(Core::Facade &facade);

    const MainWindowUi& getUi() const { return m_ui; }

    VideoView& getVideoView() { return m_videoView; }
    AlgorithmSelectionWidget& getAlgorithmSelection() { return m_algorithmSelection; }
    NotificationWidget& getNotification() { return m_notification; }
    VideoControlWidget& getVideoControl() { return m_videoControl; }

private:
    Util::QOpenGLContextWrapper m_context;
    MainWindowUi m_ui;
    AlgorithmSelectionWidget m_algorithmSelection;
    NotificationWidget m_notification;
    VideoControlWidget m_videoControl;
    VideoView m_videoView;
};



}
}
