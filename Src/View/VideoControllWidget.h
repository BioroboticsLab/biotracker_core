#ifndef BIOTRACKER3VIDEOCONTROLLWIDGET_H
#define BIOTRACKER3VIDEOCONTROLLWIDGET_H

#include <QWidget>
#include "QIcon"
#include "Interfaces/IView/IViewWidget.h"
#include "QMap"
#include "QMetaEnum"
#include "QStringListModel"
#include "View/MainWindow.h"
#include <chrono>


namespace Ui {
class VideoControllWidget;
}

class VideoControllWidget : public IViewWidget {
    Q_OBJECT

  public:
    explicit VideoControllWidget(QWidget* parent = 0, IController* controller = 0, IModel* model = 0);
    ~VideoControllWidget();

    void setSelectedView(QString str);

    void setVideoViewComboboxModel(QStringListModel* comboboxModel);

    void setTotalNumbFrames(size_t numb);
    void setCurrentFrameNumber(size_t numb);
    void setFPS(double fps);
    void setVideoControllsStates(QVector<bool> states);

    MainWindow* getMainWindow();
    void setupVideoToolbar();


  public Q_SLOTS:
    void getNotified();
    void mediumChanged(const std::string path);
    void getMaxBatchNumber(int number);

  private Q_SLOTS:
    void on_DurationChanged(int position);
    void on_PositionChanged(int position);
  //   void on_button_nextFrame_clicked();
  //   void on_button_playPause_clicked();
	// void on_button_record_clicked();
	// void on_button_record_cam_clicked();

  //   void on_button_stop_clicked();

  //   void on_button_screenshot_clicked();

  //   void on_button_previousFrame_clicked();

    void on_sld_video_sliderReleased();

    void on_sld_video_sliderMoved(int position);

	void on_sld_video_actionTriggered(int action);

    void on_doubleSpinBoxTargetFps_editingFinished();

    void on_frame_num_spin_editingFinished();


    void on_actionPlay_Pause_triggered(bool checked = false);
    void on_actionStop_triggered(bool checked = false);
    void on_actionNext_frame_triggered(bool checked = false);
    void on_actionPrev_frame_triggered(bool checked = false);
    void on_actionScreenshot_triggered(bool checked = false);
    void on_actionRecord_cam_triggered(bool checked = false);
    void on_actionRecord_all_triggered(bool checked = false);

  private:
    Ui::VideoControllWidget* ui;

    QIcon m_iconPause;
    QIcon m_iconPlay;

    size_t m_TotalNumbFrames;

	bool m_Paus;
	bool m_RecI;
	bool m_RecO;

  QAction* action_play_pause;
  QAction* action_stop;
  QAction* action_next_frame;
  QAction* action_prev_frame;
  QAction* action_rec_cam;
  QAction* action_rec;

  std::chrono::system_clock::time_point lastFpsSet;

  uint	_fpsSum = 0;
  int	_fpsCounter = 0;
};

#endif // BIOTRACKER3VIDEOCONTROLLWIDGET_H
