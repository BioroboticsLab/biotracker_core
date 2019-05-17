#ifndef CAMERADEVICE_H
#define CAMERADEVICE_H

#include <QWidget>

#include <QCamera>
#include "QCameraViewfinder"
#include "QPointer"
#include "util/types.h"
#include "util/camera/base.h"
#include <opencv2/opencv.hpp>

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

	void on_showPreviewButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_buttonBox_rejected();

Q_SIGNALS:
    void emitSelectedCameraDevice(CameraConfiguration conf);

private:
	CameraConfiguration grabUICameraConfiguration();
    void listAllCameras();

private:
    Ui::CameraDevice *ui;
	cv::VideoCapture m_capture;

    QPointer< QCamera > camera;
    QPointer< QCameraViewfinder > viewfinder;

    QList<QCameraInfo> cameras;

	int m_ximeaId;
};

#endif // CAMERADEVICE_H
