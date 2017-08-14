#include "CameraDevice.h"
#include "ui_CameraDevice.h"

#include "QCameraInfo"
#include "util/types.h"
#include <thread>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"

CameraDevice::CameraDevice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraDevice)
{
    ui->setupUi(this);
	m_ximeaId = -1;

    this->setAttribute(Qt::WA_DeleteOnClose);


    listAllCameras();
}

CameraDevice::~CameraDevice()
{
    delete ui;
}

CameraConfiguration CameraDevice::grabUICameraConfiguration() {
	int id = ui->comboBox->currentIndex();
	if (m_ximeaId == id)
		id = CV_CAP_XIAPI;

	std::string sx = ui->lineEdit->text().toStdString();
	std::string sy = ui->lineEdit_2->text().toStdString();
	std::string sf = ui->lineEdit_3->text().toStdString();
	int x, y, f;
	x = (sx == "Default" ? -1 : std::stoi(sx));
	y = (sx == "Default" ? -1 : std::stoi(sy));
	f = (sf == "Default" ? -1 : std::stoi(sf));

	CameraConfiguration conf(id, x, y, f, false, "");
	return conf;
}

void CameraDevice::on_buttonBox_accepted()
{
	CameraConfiguration conf = grabUICameraConfiguration();
    Q_EMIT emitSelectedCameraDevice(conf);

    this->close();
}

void CameraDevice::on_pushButton_clicked()
{
	int id = ui->comboBox->currentIndex();
	if (m_ximeaId == id)
	{
		m_capture.open(id);
	}
	else
	{
		id = CV_CAP_XIAPI;
		m_capture.open(id);
	}
	CameraConfiguration conf = grabUICameraConfiguration();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	int fails = 0;

	while (!m_capture.isOpened() && fails < 5) {
		m_capture.open(id);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		fails++;
	}

	if (conf._width != -1) m_capture.set(CV_CAP_PROP_FRAME_WIDTH, conf._width);
	if (conf._height != -1) m_capture.set(CV_CAP_PROP_FRAME_HEIGHT, conf._height);
	if (conf._fps != -1) m_capture.set(CV_CAP_PROP_FPS, conf._fps);
	
	if (!m_capture.isOpened()) {
		//throw device_open_error(":(");
		ui->label_NoImage->setText("Error loading camera");
	}
	else
	{
		cv::Mat mat;
		for (int i = 0; i < 10; i++) {
			m_capture.grab();
			m_capture.retrieve(mat);
		}
		cv::Mat destination;
		cv::resize(mat, destination, cv::Size(240, 180));
		QPixmap image = QPixmap::fromImage(QImage((unsigned char*)destination.data, destination.cols, destination.rows, QImage::Format_RGB888));
		ui->label_NoImage->setPixmap(image);
		m_capture.release();
	}
	
}

void CameraDevice::on_comboBox_currentIndexChanged(int index)
{
}

void CameraDevice::listAllCameras()
{
    //QCameraInfo cameraInfo(*camera);
    cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras) {

        ui->comboBox->addItem(cameraInfo.deviceName());
    }

	//Try to find ximea cameras
	cv::VideoCapture cap(CV_CAP_XIAPI);
	if (cap.isOpened()) {
		ui->comboBox->addItem("XIMEA default");
		m_ximeaId = cameras.size();
		std::cout << "XIMEA props: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << " " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
		cap.release();
	}
}

void CameraDevice::on_buttonBox_rejected()
{
    this->close();
}
