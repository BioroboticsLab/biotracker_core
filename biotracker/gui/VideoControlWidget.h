#pragma once

#include <QIcon>
#include <QWidget>

#include "biotracker/core/BioTrackerApp.h"
#include "biotracker/gui/ui_VideoControlWidget.h"
#include "biotracker/util/QtRaiiWrapper.hpp"

namespace BioTracker {
namespace Gui {

class VideoControlWidget : public QWidget {
    Q_OBJECT
  public:
    typedef Util::QtRaiiWrapper<Ui::VideoControlWidget, QWidget> VideoControlUi;

    explicit VideoControlWidget(QWidget *parent, Core::BioTrackerApp &facade,
                                VideoView *videoView);

    VideoControlUi &getUi() {
        return m_ui;
    }

    void updateWidgets();

  private:
    VideoControlUi m_ui;
    Core::BioTrackerApp &m_bioTracker;
    VideoView *m_videoView;

    QIcon m_iconPause;
    QIcon m_iconPlay;

    bool m_isPanZoomMode;

    void initConnects();
    void initShortcuts();
  public slots:
    void frameCalculated(const size_t frameNumber, const std::string &filename,
                         const double currentFps);

  private slots:
    void playPause();
    void setFrame(const size_t frame);
    void nextFrame();
    void previousFrame();
    void changeCurrentFrameByEdit();
    void takeScreenshot();
    void switchPanZoomMode();

};

}
}

