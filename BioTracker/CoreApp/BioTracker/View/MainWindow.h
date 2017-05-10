#ifndef BIOTRACKER3MAINWINDOW_H
#define BIOTRACKER3MAINWINDOW_H

//#include <boost/filesystem.hpp>
#include "Interfaces/IView/IViewMainWindow.h"

// delete
#include "GLVideoView.h"
#include "CameraDevice.h"
#include "QPointer"
#include "QStringListModel"

namespace Ui {
class MainWindow;
}

class IController;
class MainWindow : public IViewMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, IController *controller = 0, IModel *model = 0);
    ~MainWindow();

    void addVideoControllWidget(IView *widget);
    void addVideoView(IView *videoView);
    void addTrackerParameterView(IView *parameter);
    void setTrackerList(QStringListModel *trackerList);

private Q_SLOTS:
    void on_actionOpen_Video_triggered();

    void on_actionLoad_Tracker_triggered();

    void on_actionOpen_Picture_triggered();

    void on_actionLoad_tracking_data_triggered();

    void on_actionSave_tracking_data_triggered();

    void on_actionQuit_triggered();

    void on_actionOpen_Camera_triggered();

    void receiveSelectedCameraDevice(int i);

    void on_checkBox_clicked(bool checked);

private:
    Ui::MainWindow *ui;


    QPointer< CameraDevice > m_CameraDevice;
};

#endif // BIOTRACKER3MAINWINDOW_H
