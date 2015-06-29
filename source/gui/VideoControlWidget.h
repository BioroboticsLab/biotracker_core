#pragma once

#include <QIcon>
#include <QWidget>

#include "source/core/Facade.h"
#include "source/gui/ui_VideoControlWidget.h"
#include "source/util/QtRaiiWrapper.hpp"

namespace BioTracker {
namespace Gui {

class VideoControlWidget : public QWidget
{
    Q_OBJECT
public:
    typedef Util::QtRaiiWrapper<Ui::VideoControlWidget, QWidget> VideoControlUi;

    explicit VideoControlWidget(QWidget *parent, Core::Facade &facade, VideoView& videoView);

    VideoControlUi& getUi() { return m_ui; }

    void updateWidgets();

private:
    VideoControlUi m_ui;
    Core::Facade& m_facade;
    VideoView& m_videoView;

    QIcon m_iconPause;
    QIcon m_iconPlay;

    bool m_isPanZoomMode;

    void initConnects();
    void initShortcuts();

private slots:
    void playPause();
    void setFrame(const int frame);
    void nextFrame();
    void previousFrame();
    void changeCurrentFrameByEdit();
    void takeScreenshot();
    void switchPanZoomMode();
};

}
}

