#ifndef CAMERADEVICE_H
#define CAMERADEVICE_H

#include <QWidget>

#include "QCamera"
#include "QCameraViewfinder"
#include "QPointer"

namespace Ui {
class CameraDevice;
}

class CameraDevice : public QWidget
{
    Q_OBJECT

public:
    explicit CameraDevice(QWidget *parent = 0);
    ~CameraDevice();

private Q_SLOTS:
    void on_buttonBox_accepted();

    void on_comboBox_currentIndexChanged(int index);

    void on_buttonBox_rejected();

Q_SIGNALS:
    void emitSelectedCameraDevice(int i);

private:
    void listAllCameras();
    void startCamera(int i);

private:
    Ui::CameraDevice *ui;

    QPointer< QCamera > camera;
    QPointer< QCameraViewfinder > viewfinder;

    QList<QCameraInfo> cameras;
};

#endif // CAMERADEVICE_H
