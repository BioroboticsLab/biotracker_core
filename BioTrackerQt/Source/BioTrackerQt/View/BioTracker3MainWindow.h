#ifndef BIOTRACKER3MAINWINDOW_H
#define BIOTRACKER3MAINWINDOW_H

#include <boost/filesystem.hpp>
#include "Interfaces/IView/IViewMainWindow.h"

// delete
#include "BioTracker3VideoView.h"
#include "CameraDevice.h"
#include "QPointer"

namespace Ui {
class BioTracker3MainWindow;
}

class IController;
class BioTracker3MainWindow : public IViewMainWindow
{
    Q_OBJECT

public:
    explicit BioTracker3MainWindow(QWidget *parent = 0, IController *controller = 0, IModel *model = 0);
    ~BioTracker3MainWindow();

    void addVideoControllWidget(IView *widget);
    void addVideoView(IView *videoView);

private Q_SLOTS:
    void on_actionOpen_Video_triggered();

    void on_actionLoad_Tracker_triggered();

    void on_actionOpen_Picture_triggered();

    void on_actionLoad_tracking_data_triggered();

    void on_actionSave_tracking_data_triggered();

    void on_actionQuit_triggered();

    void on_actionOpen_Camera_triggered();

    void receiveSelectedCameraDevice(int i);

private:
    Ui::BioTracker3MainWindow *ui;


    QPointer< CameraDevice > m_CameraDevice;
};

#endif // BIOTRACKER3MAINWINDOW_H
