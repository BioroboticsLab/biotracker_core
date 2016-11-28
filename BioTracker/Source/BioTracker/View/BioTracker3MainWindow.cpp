#include "BioTracker3MainWindow.h"
#include "ui_BioTracker3MainWindow.h"

#include "Controller/ControllerMainWindow.h"
#include "BioTracker3VideoControllWidget.h"
#include "qfiledialog.h"
#include "QLayout"

#include "View/GraphicsView.h"




BioTracker3MainWindow::BioTracker3MainWindow(QWidget *parent, IController *controller, IModel *model) :
    IViewMainWindow(parent, controller, model),
    ui(new Ui::BioTracker3MainWindow)
{
    ui->setupUi(this);
}

BioTracker3MainWindow::~BioTracker3MainWindow()
{
    delete ui;
}

void BioTracker3MainWindow::addVideoControllWidget(IView *widget)
{
    QLayout *layout = new QGridLayout(this);
    layout->addWidget(dynamic_cast<QWidget *>(widget));
    ui->videoControls->setLayout(layout);
}

void BioTracker3MainWindow::addVideoView(IView *videoView)
{
    GraphicsView *graphView = dynamic_cast<GraphicsView *>(videoView);
    graphView->setParent(ui->trackingArea);
    ui->videoViewLayout->addWidget(graphView);
    // gl widget
    //dynamic_cast<BioTracker3VideoView *>(videoView)->setParent(ui->trackingArea);
    //ui->videoViewLayout->addWidget(dynamic_cast<BioTracker3VideoView *>(videoView));
}

void BioTracker3MainWindow::on_actionOpen_Video_triggered()
{
    static const QString videoFilter("Video files (*.avi *.wmv *.mp4 *.mkv *.mov)");

    QString filename = QFileDialog::getOpenFileName(this,
                             "Open video", "", videoFilter);

    if (!filename.isEmpty()) {
        dynamic_cast<ControllerMainWindow *> (getController())->loadVideo(filename);
    }
}

void BioTracker3MainWindow::on_actionLoad_Tracker_triggered()
{
    static const QString pluginFilter("BioTracker Tracking Plugin files (*.so *.dll *.dylib)");

    QString filename = QFileDialog::getOpenFileName(this,
                             "Open BioTracker Tracking Plugin", "", pluginFilter);

    if (!filename.isEmpty()) {
        qobject_cast<ControllerMainWindow *> (getController())->loadTracker(filename);
    }
}

void BioTracker3MainWindow::on_actionOpen_Picture_triggered()
{
    static const QString imageFilter(
        "image files (*.png *.jpg *.jpeg *.gif *.bmp *.jpe *.ppm *.tiff *.tif *.sr *.ras *.pbm *.pgm *.jp2 *.dib)");

    std::vector<boost::filesystem::path> files;
    for (QString const &path : QFileDialog::getOpenFileNames(this,
            "Open image files", "", imageFilter)) {
        files.push_back(boost::filesystem::path(path.toStdString()));
    }

    if (!files.empty()) {
        qobject_cast<ControllerMainWindow *> (getController())->loadPictures(files);
    }
}

void BioTracker3MainWindow::on_actionLoad_tracking_data_triggered()
{

}

void BioTracker3MainWindow::on_actionSave_tracking_data_triggered()
{

}

void BioTracker3MainWindow::on_actionQuit_triggered()
{

}

void BioTracker3MainWindow::on_actionOpen_Camera_triggered()
{
    m_CameraDevice = new CameraDevice();

    m_CameraDevice->show();

    QObject::connect(m_CameraDevice, &CameraDevice::emitSelectedCameraDevice, this, &BioTracker3MainWindow::receiveSelectedCameraDevice);
}

void BioTracker3MainWindow::receiveSelectedCameraDevice(int i)
{
    qobject_cast<ControllerMainWindow *> (getController())->loadCameraDevice(i);

}
