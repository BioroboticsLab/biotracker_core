#pragma once

#include <QIcon>
#include <QWidget>

#include "biotracker/BioTrackerApp.h"
#include "ui_VideoControlWidget.h"
#include "biotracker/util/QtRaiiWrapper.hpp"
#include "VideoView.h"
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

  public Q_SLOTS:
    void fileOpened(const std::string filename, const size_t totalFrames);

  private:
    VideoControlUi m_ui;
    Core::BioTrackerApp &m_bioTracker;
    VideoView *m_videoView;

    QIcon m_iconPause;
    QIcon m_iconPlay;

    bool m_isPanZoomMode;
    bool m_videoSliderChanged;
    bool m_sliderVideoWasRunning;

    void initConnects();
    void initShortcuts();
  public Q_SLOTS:
    void frameCalculated(const size_t frameNumber, const std::string filename,
                         const double currentFps);

  private Q_SLOTS:
    void playPause();
    void setFrame(const size_t frame);
    void nextFrame();
    void previousFrame();
    void changeCurrentFrameByEdit();
    void takeScreenshot();
    void switchPanZoomMode();
    void sliderRender();
    void videoSliderChanged(const size_t frame);
    void videoSliderReleased();
    void videoSliderPressed();
    void speedSliderReleased();
};

}
}

