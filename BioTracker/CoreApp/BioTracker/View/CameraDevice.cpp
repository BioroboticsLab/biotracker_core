#include "CameraDevice.h"
#include "ui_CameraDevice.h"

#include "QCameraInfo"

CameraDevice::CameraDevice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraDevice)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);

    listAllCameras();
}

CameraDevice::~CameraDevice()
{
    delete ui;
    camera->stop();
}

void CameraDevice::on_buttonBox_accepted()
{
    int i = ui->comboBox->currentIndex();

    Q_EMIT emitSelectedCameraDevice(i);

    this->close();
}

void CameraDevice::on_comboBox_currentIndexChanged(int index)
{
    startCamera(index);
}

void CameraDevice::listAllCameras()
{
    //QCameraInfo cameraInfo(*camera);
    cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras) {

        ui->comboBox->addItem(cameraInfo.deviceName());
    }

    startCamera(0);
}

void CameraDevice::startCamera(int i)
{
    if(!cameras.empty()) {

        camera = new QCamera(cameras.at(i), this);
        camera->setParent(this);

        viewfinder = new QCameraViewfinder(this);
        viewfinder->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);


        QPointer< QGridLayout > layout = new QGridLayout(this);
        ui->widget->setLayout(layout);
        layout->addWidget(viewfinder);

        camera->setViewfinder(viewfinder);
        camera->start();
    }

}

void CameraDevice::on_buttonBox_rejected()
{
    this->close();
}
