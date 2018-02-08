#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Controller/ControllerMainWindow.h"
#include "View/CoreParameterView.h"
#include "VideoControllWidget.h"
#include "qfiledialog.h"
#include "QLayout"

#include "View/GraphicsView.h"
#include "View/AnnotationsView.h"
#include "Model/null_Model.h"
#include "Controller/null_Controller.h"

#include "QGraphicsObject"
#include <QTableWidget>
#include <QTableWidgetItem>

#include "qtextedit.h"
#include <qmessagebox.h>


MainWindow::MainWindow(QWidget* parent, IController* controller, IModel* model) :
    IViewMainWindow(parent, controller, model),
    ui(new Ui::MainWindow) {
    _previouslySelectedTracker = "";
	_currentParameterView = 0;
	_currentCoreParameterView = 0;
	_currentElementView = 0;
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::addVideoControllWidget(IView* widget) {
    QLayout* layout = new QGridLayout();
    layout->addWidget(dynamic_cast<QWidget*>(widget));
    ui->videoControls->setLayout(layout);
}

void MainWindow::addVideoView(IView* videoView) {
	m_graphView = dynamic_cast<GraphicsView*>(videoView);
	m_graphView->setParent(ui->trackingArea);

    ui->videoViewLayout->addWidget(m_graphView);
}

void MainWindow::addTrackerElementsView(IView *elemView)
{
	if (_currentElementView && _currentCoreView) {
		_currentElementView->setParent(0); 
		_currentCoreView->setParent(0);
		m_graphView->removeGraphicsItem(_currentElementView);
		m_graphView->removeGraphicsItem(_currentCoreView);
	}
	QGraphicsObject *graphObj = dynamic_cast<QGraphicsObject *>(elemView);
	graphObj->setParent(ui->trackingArea);

	m_graphView->addGraphicsItem(graphObj);
	QRectF viewSceneRect = m_graphView->sceneRect();

	_currentElementView = graphObj;
}

void MainWindow::addCoreElementsView(IView * coreView)
{
	QGraphicsObject *graphObj = dynamic_cast<QGraphicsObject *>(coreView);
	graphObj->setParent(ui->trackingArea);

	m_graphView->addGraphicsItem(graphObj);
	_currentCoreView = graphObj;
}

void MainWindow::addNotificationBrowser(IView * notificationBrowser)
{
	QWidget* notificationWidget = dynamic_cast<QWidget*>(notificationBrowser);
	if (notificationWidget) {
		notificationWidget->updateGeometry();
		
		ui->toolBox->addItem(notificationWidget, QString("Notifications"));

		notificationWidget->setVisible(1);
	}
}

void MainWindow::addTrackerParameterView(IView *parameter) 
{
	if (_currentCoreParameterView) {
		CoreParameterView* v = static_cast<CoreParameterView*>(_currentCoreParameterView);
		QWidget* w = v->getTrackerHook();

		if (_currentParameterView) {
			dynamic_cast<QWidget*>(_currentParameterView)->setParent(0);
		}

		QWidget* pluginParameter = dynamic_cast<QWidget*>(parameter);
		dynamic_cast<QTabWidget*>(w)->removeTab(0);
		dynamic_cast<QTabWidget*>(w)->insertTab(0, pluginParameter, "tracker");
		
	}
	else {
		qDebug() << "Error adding tracker parameter";
		assert(false);
	}
}

void MainWindow::addCoreParameterView(IView * coreParameterView)
{

	QWidget* coreParameter = dynamic_cast<QWidget*>(coreParameterView);
	if (coreParameter) {
		ui->widgetParameterAreaOuterCanvas->updateGeometry();
		coreParameter->updateGeometry();
		coreParameter->setContentsMargins(QMargins(0, 0, 0, 0));
		coreParameter->setParent(ui->widgetParameterAreaOuterCanvas);

		QHBoxLayout* hLayout = new QHBoxLayout;
		hLayout->addWidget(coreParameter, 100, 0);
		hLayout->setContentsMargins(QMargins(0,0,0,0)); //left, top, right, bottom
		hLayout->setMargin(0);

		ui->widgetParameterAreaOuterCanvas->setLayout(hLayout);

		coreParameter->setVisible(1);
		_currentCoreParameterView = coreParameterView;
	}
}

void MainWindow::on_comboBox_TrackerSelect_currentIndexChanged(QString s) {
	QString ct = ui->comboBox_TrackerSelect->currentText();
    if (!ct.isEmpty() && _previouslySelectedTracker != ct) {
        _previouslySelectedTracker = ct;
        Q_EMIT selectPlugin(ct);
    }
}

void MainWindow::setTrackerList(QStringListModel* trackerList, QString current) {
    ui->comboBox_TrackerSelect->setModel(trackerList);
	ui->comboBox_TrackerSelect->setCurrentText(current);
}

void MainWindow::setCursorPositionLabel(QPoint pos)
{
	QString posString = QString("%1, %2").arg(QString::number(pos.x()), QString::number(pos.y()));
	ui->cursorPosition->setText(posString);
}

void MainWindow::activeTrackingCheckBox() {
    ui->checkBox_TrackingActivated->setEnabled(true);
}

void MainWindow::deactivateTrackingCheckBox() {
    ui->checkBox_TrackingActivated->setEnabled(false);
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
    static const QString pluginFilter("BioTracker Tracking Plugin files (*.tracker.so *.tracker.dll *.tracker.dylib)");

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

void MainWindow::on_actionLoad_trackingdata_triggered() {
	static const QString imageFilter(
		"tracking data files (*.csv *.dat *.json)");

	std::vector<boost::filesystem::path> files;
	for (QString const& path : QFileDialog::getOpenFileNames(this,
		"Open image files", "", imageFilter, 0, QFileDialog::DontUseNativeDialog)) {
		files.push_back(boost::filesystem::path(path.toStdString()));
	}

	if (!files.empty()) {
		qobject_cast<ControllerMainWindow*> (getController())->loadTrajectoryFile(files[0].string());
	}
}

void MainWindow::on_actionOpen_Camera_triggered() {
    m_CameraDevice = new CameraDevice();

    m_CameraDevice->show();

    QObject::connect(m_CameraDevice, &CameraDevice::emitSelectedCameraDevice, this, &MainWindow::receiveSelectedCameraDevice);
}

void MainWindow::on_actionUndo_triggered()
{
	qobject_cast<ControllerMainWindow*> (getController())->emitUndoCommand();
}

void MainWindow::on_actionRedo_triggered()
{
	qobject_cast<ControllerMainWindow*> (getController())->emitRedoCommand();
}

void MainWindow::on_actionShowActionList_triggered()
{
	qobject_cast<ControllerMainWindow*> (getController())->emitShowActionListCommand();
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

void MainWindow::on_actionQuit_triggered() {
	qobject_cast<ControllerMainWindow*> (getController())->exit();
}


//////////////////////////////////Extras//////////////////////////////

void MainWindow::on_actionSettings_triggered() {
	m_SettingsWindow = new SettingsWindow();

	m_SettingsWindow->show();
}

void MainWindow::on_actionInfo_triggered() {
    std::string version = "";
    version = "BioTracker3\n\n";

    version = "Version: ";
    version += GIT_HASH;
    version += +"\n\n";

    version += "External libraries used:\n\n";
    std::string qt = MyQT_VERSION;
    version += "QT " + qt + "\n";

    std::string cv = MyCV_VERSION;
    version += "OpenCV " + cv + "\n";

    std::string boost = MyBT_VERSION;
    version += "Boost " + boost + "\n";

    version += "\n\nPublished under LGPL licence";

    int ret = QMessageBox::information(this, tr("BioTracker"),
        tr( version.c_str()),
        QMessageBox::Ok);
}

void MainWindow::on_actionShortcuts_triggered() {

	//TODO import this from file
    std::pair<QString, QString> scUndo (QString("ctrl + z"), QString("undo"));
    std::pair<QString, QString> scRedo (QString("ctrl + y"), QString("redo"));
    std::pair<QString, QString> scSel (QString("ctrl + a"), QString("select all"));
    std::pair<QString, QString> scPlay (QString("Space"), QString("play"));
    std::pair<QString, QString> scNext (QString("Left Arrow"), QString("Previous Frame"));
    std::pair<QString, QString> scPrev (QString("Right Arrow"), QString("Next Frame"));
    std::pair<QString, QString> scStop (QString("ctrl + Space"), QString("Stop"));
    std::pair<QString, QString> scAL (QString("alt + l"), QString("add label annotation"));
    std::pair<QString, QString> scAA (QString("alt + a"), QString("add arrow annotation"));
    std::pair<QString, QString> scAE (QString("alt + e"), QString("add ellipse annotation"));
    std::pair<QString, QString> scAR (QString("alt + r"), QString("add rect annotation"));
    std::pair<QString, QString> scADel (QString("alt + delete"), QString("delete selected annotation"));

	std::map<QString,QString> scMap;

	scMap.insert(scUndo);
	scMap.insert(scRedo);
	scMap.insert(scSel);
	scMap.insert(scPlay);
	scMap.insert(scNext);
	scMap.insert(scPrev);
	scMap.insert(scStop);
	scMap.insert(scAL);
	scMap.insert(scAA);
	scMap.insert(scAE);
	scMap.insert(scAR);
	scMap.insert(scADel);

	QMap<QString, QString> scQMap = QMap<QString, QString>(scMap);

	QTableWidget* scTable = new QTableWidget();
	scTable->setRowCount(1);
	scTable->setColumnCount(2);

	scTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Shortcut"));
	scTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Description"));
	scTable->verticalHeader()->hide();

	QMapIterator<QString, QString> sc(scQMap);
	while (sc.hasNext()) {
		sc.next();
		QTableWidgetItem* scKey = new QTableWidgetItem(sc.key());
		QTableWidgetItem* scKeyInfo = new QTableWidgetItem(sc.value());
		scKey->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		scKeyInfo->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

		scTable->setItem(scTable->rowCount()-1 , 0, scKey);
		scTable->setItem(scTable->rowCount()-1 , 1, scKeyInfo);

		if(sc.hasNext()){
			scTable->insertRow(scTable->rowCount());
		}
	}

 	//scTable->horizontalHeader()->setStretchLastSection( true ); 
	scTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	QWidget* outerWidget = new QWidget();
	outerWidget->resize(scTable->size());
	QVBoxLayout* vLayout = new QVBoxLayout();

	qDebug() << scTable->size();
	qDebug() << scTable->baseSize();
	qDebug() << scTable->frameSize();


	vLayout->addWidget(scTable);

	outerWidget->setLayout(vLayout);

	qDebug() << scTable->size();
	qDebug() << scTable->baseSize();
	qDebug() << scTable->frameSize();


	outerWidget->show();


}




