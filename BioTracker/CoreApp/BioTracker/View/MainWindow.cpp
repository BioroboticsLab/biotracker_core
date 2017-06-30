#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Controller/ControllerMainWindow.h"
#include "VideoControllWidget.h"
#include "qfiledialog.h"
#include "QLayout"

#include "View/GraphicsView.h"

#include "QGraphicsObject"//MARKER



MainWindow::MainWindow(QWidget* parent, IController* controller, IModel* model) :
    IViewMainWindow(parent, controller, model),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::addVideoControllWidget(IView* widget) {
    QLayout* layout = new QGridLayout(this);
    layout->addWidget(dynamic_cast<QWidget*>(widget));
    ui->videoControls->setLayout(layout);
}

void MainWindow::addVideoView(IView* videoView) {
	m_graphView = dynamic_cast<GraphicsView*>(videoView);
	m_graphView->setParent(ui->trackingArea);
    ui->videoViewLayout->addWidget(m_graphView);
    // gl widget
    //dynamic_cast<BioTracker3VideoView *>(videoView)->setParent(ui->trackingArea);
    //ui->videoViewLayout->addWidget(dynamic_cast<BioTracker3VideoView *>(videoView));
}

void MainWindow::addTrackerElementsView(IView *elemView)
{
	QGraphicsObject *graphObj = dynamic_cast<QGraphicsObject *>(elemView);
	graphObj->setParent(ui->trackingArea);
	m_graphView->addGraphicsItem(graphObj);
}

void MainWindow::addTrackerParameterView(IView *parameter) 
{
    dynamic_cast<QWidget*>(parameter)->setParent(this);

    ui->scrollArea->setWidget(dynamic_cast<QWidget*>(parameter));
    ui->scrollArea->setWidgetResizable(true);
}

void MainWindow::setTrackerList(QStringListModel* trackerList) {
    ui->comboBox_TrackerSelect->setModel(trackerList);
}

void MainWindow::activeTrackingCheckBox() {
    ui->checkBox_TrackingActivated->setEnabled(true);
}

void MainWindow::deactivateTrackingCheckBox() {
    ui->checkBox_TrackingActivated->setEnabled(true);
}

void MainWindow::on_actionOpen_Video_triggered() {
    static const QString videoFilter("Video files (*.avi *.wmv *.mp4 *.mkv *.mov)");

    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Open video", "", videoFilter, 0, QFileDialog::DontUseNativeDialog);

    if (!filename.isEmpty()) {
        dynamic_cast<ControllerMainWindow*> (getController())->loadVideo(filename);
    }
}

void MainWindow::on_actionLoad_Tracker_triggered() {
    static const QString pluginFilter("BioTracker Tracking Plugin files (*.so *.dll *.dylib)");

    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Open BioTracker Tracking Plugin", "", pluginFilter, 0, QFileDialog::DontUseNativeDialog);

    if (!filename.isEmpty()) {
        qobject_cast<ControllerMainWindow*> (getController())->loadTracker(filename);
    }
}

void MainWindow::on_actionOpen_Picture_triggered() {
    static const QString imageFilter(
        "image files (*.png *.jpg *.jpeg *.gif *.bmp *.jpe *.ppm *.tiff *.tif *.sr *.ras *.pbm *.pgm *.jp2 *.dib)");

    std::vector<boost::filesystem::path> files;
    for (QString const& path : QFileDialog::getOpenFileNames(this,
                                                             "Open image files", "", imageFilter, 0, QFileDialog::DontUseNativeDialog)) {
        files.push_back(boost::filesystem::path(path.toStdString()));
    }

    if (!files.empty()) {
        qobject_cast<ControllerMainWindow*> (getController())->loadPictures(files);
    }
}

void MainWindow::on_actionLoad_tracking_data_triggered() {

}

void MainWindow::on_actionSave_tracking_data_triggered() {

}

void MainWindow::on_actionQuit_triggered() {

}

void MainWindow::on_actionOpen_Camera_triggered() {
    m_CameraDevice = new CameraDevice();

    m_CameraDevice->show();

    QObject::connect(m_CameraDevice, &CameraDevice::emitSelectedCameraDevice, this, &MainWindow::receiveSelectedCameraDevice);
}

void MainWindow::receiveSelectedCameraDevice(CameraConfiguration conf) {
    qobject_cast<ControllerMainWindow*> (getController())->loadCameraDevice(conf);

}

void MainWindow::on_checkBox_TrackingActivated_stateChanged(int arg1) {
    if(arg1 == Qt::Checked)
        qobject_cast<ControllerMainWindow*> (getController())->activeTracking();

    if(arg1 == Qt::Unchecked)
        qobject_cast<ControllerMainWindow*> (getController())->deactiveTrackring();
}


//////////////////////////////////Extras//////////////////////////////

void MainWindow::on_actionSettings_triggered() {
	m_SettingsWindow = new SettingsWindow();

	m_SettingsWindow->show();

	//QObject::connect(m_CameraDevice, &CameraDevice::emitSelectedCameraDevice, this, &MainWindow::receiveSelectedCameraDevice);
}


