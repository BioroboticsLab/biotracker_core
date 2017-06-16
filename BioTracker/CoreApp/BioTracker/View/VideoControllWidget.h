#ifndef BIOTRACKER3VIDEOCONTROLLWIDGET_H
#define BIOTRACKER3VIDEOCONTROLLWIDGET_H

#include <QWidget>
#include "QIcon"
#include "Interfaces/IView/IViewWidget.h"
#include "QMap"
#include "QMetaEnum"
#include "QStringListModel"


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


  public Q_SLOTS:
    void getNotified();

  private Q_SLOTS:
    void on_DurationChanged(int position);
    void on_PositionChanged(int position);
    void on_button_nextFrame_clicked();
    void on_button_playPause_clicked();
	void on_pushButton_clicked();

    void on_button_stop_clicked();

    void on_button_previousFrame_clicked();


    void on_comboBoxSelectedView_currentTextChanged(const QString& arg1);

    void on_sld_video_sliderReleased();

    void on_sld_video_sliderMoved(int position);

  private:
    Ui::VideoControllWidget* ui;

    QIcon m_iconPause;
    QIcon m_iconPlay;

    size_t m_TotalNumbFrames;

    bool m_Paus;
};

#endif // BIOTRACKER3VIDEOCONTROLLWIDGET_H
