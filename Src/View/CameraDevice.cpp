#include "CameraDevice.h"
#include "ui_CameraDevice.h"

#include "QCameraInfo"
#include "util/types.h"
#include "util/camera/base.h"
#if HAS_PYLON
#include "util/camera/pylon.h"
#endif
#include <thread>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"

CameraDevice::CameraDevice(QWidget *parent) : QWidget(parent),
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

CameraConfiguration CameraDevice::grabUICameraConfiguration()
{
    auto camera_selector = ui->comboBox->currentData().value<CameraSelector>();

    std::string sx = ui->lineEdit->text().toStdString();
    std::string sy = ui->lineEdit_2->text().toStdString();
    std::string sf = ui->lineEdit_3->text().toStdString();
    int x, y, f;
    x = (sx == "Default" ? -1 : std::stoi(sx));
    y = (sy == "Default" ? -1 : std::stoi(sy));
    f = (sf == "Default" ? -1 : std::stoi(sf));

    CameraConfiguration conf(camera_selector, x, y, f, false, "");
    return conf;
}

void CameraDevice::on_buttonBox_accepted()
{
    CameraConfiguration conf = grabUICameraConfiguration();
    Q_EMIT emitSelectedCameraDevice(conf);

    this->close();
}

void CameraDevice::on_showPreviewButton_clicked()
{
    auto conf = grabUICameraConfiguration();

    cv::Mat image;
    switch (conf._selector.type)
    {
    case CameraType::OpenCV:
    {
        m_capture.open(conf._selector.index);

        for (auto num_tries = 0; !m_capture.isOpened() && num_tries < 50; ++num_tries)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (conf._width != -1)
            m_capture.set(CV_CAP_PROP_FRAME_WIDTH, conf._width);
        if (conf._height != -1)
            m_capture.set(CV_CAP_PROP_FRAME_HEIGHT, conf._height);
        if (conf._fps != -1)
            m_capture.set(CV_CAP_PROP_FPS, conf._fps);

        if (!m_capture.isOpened())
        {
            ui->label_NoImage->setText("Error loading camera");
            return;
        }

        cv::Mat mat;
        for (auto i = 0; i < 10; ++i)
        {
            m_capture.grab();
            m_capture.retrieve(mat);
        }

        auto height = conf._height != -1 ? conf._height : ui->label_NoImage->height();
        auto ar = mat.rows ? static_cast<float>(mat.cols) / mat.rows : 1;
        auto width = conf._width != -1 ? conf._width : static_cast<int>(height * ar);

        cv::Mat scaled;
        cv::resize(mat, scaled, cv::Size{width, height});
        if (scaled.type() == CV_8UC1)
            cv::cvtColor(scaled, scaled, cv::COLOR_GRAY2RGB);
        cv::cvtColor(scaled, scaled, cv::COLOR_BGR2RGB);

        ui->label_NoImage->setPixmap(QPixmap::fromImage(QImage(scaled.data, scaled.cols, scaled.rows, static_cast<int>(scaled.step1()), QImage::Format_RGB888)));
        m_capture.release();
        break;
    }
#if HAS_PYLON
    case CameraType::Pylon:
    {
        Pylon::PylonAutoInitTerm pylon;
        auto camera = Pylon::CInstantCamera{getPylonDevice(conf._selector.index)};
        camera.Open();
        for (auto num_tries = 0; !camera.IsOpen() && num_tries < 50; ++num_tries)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (!camera.IsOpen())
        {
            ui->label_NoImage->setText("Error loading camera");
            return;
        }

        Pylon::CGrabResultPtr grab_result;
        camera.GrabOne(5000, grab_result);
        auto view = toOpenCV(grab_result);

        auto height = conf._height != -1 ? conf._height : ui->label_NoImage->height();
        auto ar = view.rows ? static_cast<float>(view.cols) / view.rows : 1;
        auto width = conf._width != -1 ? conf._width : static_cast<int>(height * ar);

        cv::Mat scaled;
        cv::resize(view, scaled, cv::Size{width, height});
        if (scaled.type() == CV_8UC1)
            cv::cvtColor(scaled, scaled, cv::COLOR_GRAY2RGB);

        ui->label_NoImage->setPixmap(QPixmap::fromImage(QImage(scaled.data, scaled.cols, scaled.rows, static_cast<int>(scaled.step1()), QImage::Format_RGB888)));
        break;
    }
#endif
    default:
        throw std::logic_error("Missing preview implementation");
    }
}

void CameraDevice::on_comboBox_currentIndexChanged(int index)
{
}

void CameraDevice::listAllCameras()
{
    cameras = QCameraInfo::availableCameras();
    for (int index = 0; index < cameras.size(); ++index)
    {
        ui->comboBox->addItem(
            cameras[index].description(),
            QVariant::fromValue(CameraSelector{CameraType::OpenCV, index}));
    }

    {
        cv::VideoCapture ximea_camera(CV_CAP_XIAPI);
        if (ximea_camera.isOpened())
        {
            ui->comboBox->addItem(
                "XIMEA default",
                QVariant::fromValue(CameraSelector{CameraType::OpenCV, CV_CAP_XIAPI}));
        }
    }

#if HAS_PYLON
    {
        Pylon::PylonAutoInitTerm pylon;

        auto &factory = Pylon::CTlFactory::GetInstance();
        Pylon::DeviceInfoList_t devices;
        factory.EnumerateDevices(devices);

        for (int index = 0; index < devices.size(); ++index)
        {
            ui->comboBox->addItem(
                QString{devices[index].GetFriendlyName()},
                QVariant::fromValue(CameraSelector{CameraType::Pylon, index}));
        }
    }
#endif
}

void CameraDevice::on_buttonBox_rejected()
{
    this->close();
}
