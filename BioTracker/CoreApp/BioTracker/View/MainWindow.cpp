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
#include <QtWidgets/QHeaderView>


#include "qtextedit.h"
#include <qmessagebox.h>

#include "qdesktopwidget.h"

#include "QToolButton"



MainWindow::MainWindow(QWidget* parent, IController* controller, IModel* model) :
    IViewMainWindow(parent, controller, model),
    ui(new Ui::MainWindow) {
    _previouslySelectedTracker = "";
	_currentParameterView = 0;
	_currentCoreParameterView = 0;
	_currentElementView = 0;
    ui->setupUi(this);

	//add cursor position label
	QLabel* _cursorPosition = new QLabel("0,0");
	_cursorPosition->setObjectName("_cursorPosition");
 	statusBar()->insertPermanentWidget(0,_cursorPosition);

	//setup toolbars
	setupUpperToolBar();

	//resize to full size
	//QWidget::showFullScreen();
	QWidget::showMaximized();

	//view actions
	//QAction* dockWidgetHider = ui->dockWidgetAlgorithm->toggleViewAction();
	//dockWidgetHider->setText(ui->dockWidgetAlgorithm->isVisible()?"Hide right panel":"Show right panel");
	//ui->menuView->addAction(dockWidgetHider);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupUpperToolBar() {
	//add tracker list to upper toolbar
	//ui->toolBarMenu->addWidget(ui->comboBox_TrackerSelect);
	//
	_trackerActivator = new SwitchButton();
	QObject::connect(_trackerActivator , &SwitchButton::emitSetTracking, this, &MainWindow::receiveSetTracking, Qt::DirectConnection);
	//QAction* activatorAction = ui->toolBarMenu->addWidget(_trackerActivator);

	//choose tracker groupbox
	QGroupBox* chooseTrackerBox = new QGroupBox("Choose tracker");
	QFont font1 = QFont();
	font1.setPointSize(8);
	font1.setBold(true);
	chooseTrackerBox->setFont(font1);
	QHBoxLayout* chooseTrackerBoxLayout = new QHBoxLayout;
	chooseTrackerBoxLayout->addWidget(ui->comboBox_TrackerSelect);
	chooseTrackerBoxLayout->addWidget(_trackerActivator);

	chooseTrackerBox->setLayout(chooseTrackerBoxLayout);
	ui->toolBarMenu->addWidget(chooseTrackerBox);

	//media groupbox
	QGroupBox* mediaBox = new QGroupBox("Load media");
	QFont font = QFont();
	font.setPointSize(8);
	font.setBold(true);
	mediaBox->setFont(font);
	QHBoxLayout* mediaBoxLayout = new QHBoxLayout;
	mediaBoxLayout->setContentsMargins(0,0,0,0);
	//mediaBoxLayout->setSpacing(0);
	QToolButton* cameraButton = new QToolButton;
	cameraButton->setIconSize(QSize(24,24));
	cameraButton->setDefaultAction(ui->actionOpen_Camera);
	cameraButton->setAutoRaise(true);

	QToolButton* videoButton = new QToolButton;
	videoButton->setIconSize(QSize(24,24));
	videoButton->setDefaultAction(ui->actionOpen_Video);
	videoButton->setAutoRaise(true);

	QToolButton* imageButton = new QToolButton;
	imageButton->setIconSize(QSize(24,24));
	imageButton->setDefaultAction(ui->actionOpen_Picture);
	imageButton->setAutoRaise(true);

	mediaBoxLayout->addWidget(videoButton);
	mediaBoxLayout->addWidget(imageButton);
	mediaBoxLayout->addWidget(cameraButton);

	mediaBox->setLayout(mediaBoxLayout);
	ui->toolBarMenu->addWidget(mediaBox);


	////////trackerbox
	QFrame* trackerBox = new QFrame;
	trackerBox->setObjectName("trackerBox");
	trackerBox->setStyleSheet("#trackerBox { border: 1px solid #e5e5e5; }");
	QHBoxLayout* trackerBoxLayout = new QHBoxLayout;
	trackerBoxLayout->setContentsMargins(5,0,5,0);

	QToolButton* trackerButton = new QToolButton;
	trackerButton->setIconSize(QSize(24,24));
	trackerButton->setDefaultAction(ui->actionLoad_Tracker);
	trackerButton->setAutoRaise(true);

	trackerBoxLayout->addWidget(trackerButton);
	trackerBox->setLayout(trackerBoxLayout);
	ui->toolBarMenu->addWidget(trackerBox);


	////////loadSaveBox
	QWidget* loadSaveBox = new QWidget;
	loadSaveBox->setStyleSheet("background-color:#e5e5e5;"); 
	QHBoxLayout* loadSaveBoxLayout = new QHBoxLayout;
	loadSaveBoxLayout->setContentsMargins(5,0,5,0);

	QToolButton* loadFileButton = new QToolButton;
	loadFileButton->setIconSize(QSize(24,24));
	loadFileButton->setDefaultAction(ui->actionLoad_trackingdata);
	loadFileButton->setAutoRaise(true);
	
	QToolButton* saveFileButton = new QToolButton;
	saveFileButton->setIconSize(QSize(24,24));
	saveFileButton->setDefaultAction(ui->actionSave_trackingdata);
	saveFileButton->setAutoRaise(true);

	loadSaveBoxLayout->addWidget(loadFileButton);
	loadSaveBoxLayout->addWidget(saveFileButton);
	loadSaveBox->setLayout(loadSaveBoxLayout);
	ui->toolBarMenu->addWidget(loadSaveBox);
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
		ui->widget_alg->updateGeometry();
		coreParameter->updateGeometry();
		coreParameter->setContentsMargins(QMargins(0, 0, 0, 0));
		coreParameter->setParent(ui->widget_alg);

		QHBoxLayout* hLayout = new QHBoxLayout;
		hLayout->addWidget(coreParameter, 100, 0);
		hLayout->setContentsMargins(QMargins(0,0,0,0)); //left, top, right, bottom
		hLayout->setMargin(0);

		ui->widget_alg->setLayout(hLayout);

		coreParameter->setVisible(1);
		_currentCoreParameterView = coreParameterView;


		
		//resize rightpanel
		QList<int> splitterSizes;
		splitterSizes << 20000 << 10000; //2:1 TODO make this fixed (right panel fixed start size)
		ui->mainSplitter->setSizes(splitterSizes);

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
	if(_cursorPosition){
		_cursorPosition->setText(posString);
	}

	QLabel* currentChild = statusBar()->findChild<QLabel*>("_cursorPosition");
	currentChild->setText(posString);
	
}

void MainWindow::activeTrackingCheckBox() {
    //ui->checkBox_TrackingActivated->setEnabled(true);
	_trackerActivator->setEnabled(true);
}

void MainWindow::deactivateTrackingCheckBox() {
    //ui->checkBox_TrackingActivated->setEnabled(false);
	_trackerActivator->setEnabled(false);
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

void MainWindow::on_actionQuit_triggered() {
	qobject_cast<ControllerMainWindow*> (getController())->exit();
}

void MainWindow::on_rightPanelViewControllerButton_clicked(){
	QList<int> splitterSizes = QList<int> ();
	if(ui->widgetParameterAreaOuterCanvas->isVisible()){
		_lastRightPanelWidth = ui->mainSplitter->sizes()[1]; //save last width to restore on show
		_lastVideoWidgetWidth = ui->mainSplitter->sizes()[0];

		ui->widgetParameterAreaOuterCanvas->hide();
		splitterSizes << 20000 << ui->rightPanelViewControllerWidget->minimumSizeHint().width();

		ui->rightPanelViewControllerButton->setText("<");	
	}
	else{
		ui->widgetParameterAreaOuterCanvas->show();
		splitterSizes << _lastVideoWidgetWidth << _lastRightPanelWidth;
		ui->rightPanelViewControllerButton->setText(">");	
	}
	ui->mainSplitter->setSizes(splitterSizes);
}

void MainWindow::on_bottomPanelViewControllerButton_clicked(){
	ui->videoControls->setVisible(ui->bottomPanelViewControllerButton->text() == "^");
	ui->bottomPanelViewControllerButton->setText(ui->bottomPanelViewControllerButton->text() == "v"?"^":"v");
}

void MainWindow::receiveSetTracking(bool toggle){
	if(toggle){
		qobject_cast<ControllerMainWindow*> (getController())->activeTracking();
	}
	else{
		qobject_cast<ControllerMainWindow*> (getController())->deactiveTrackring();
	}
}

//////////////////////////Left toolbar actions///////////////////////////
void MainWindow::on_actionAdd_Track_triggered(){
	qobject_cast<ControllerMainWindow*> (getController())->emitAddTrack();
}
void MainWindow::on_actionDelete_selected_entity_triggered(){
	qobject_cast<ControllerMainWindow*> (getController())->emitDeleteSelectedTracks();
}
void MainWindow::on_actionChange_the_border_color_triggered(){
	qobject_cast<ControllerMainWindow*> (getController())->emitChangeColorBorder();
}
void MainWindow::on_actionChange_the_fill_color_triggered(){
	qobject_cast<ControllerMainWindow*> (getController())->emitChangeColorFill();
}
void MainWindow::on_actionAdd_label_Annotation_triggered(){
	qobject_cast<ControllerMainWindow*> (getController())->emitAddLabelAnno();
}
void MainWindow::on_actionAdd_rectangular_annotation_triggered(){
	qobject_cast<ControllerMainWindow*> (getController())->emitAddRectAnno();
}
void MainWindow::on_actionAdd_arrow_annotation_triggered(){
	qobject_cast<ControllerMainWindow*> (getController())->emitAddArrAnno();
}
void MainWindow::on_actionAdd_elliptical_annotation_triggered(){
	qobject_cast<ControllerMainWindow*> (getController())->emitAddEllAnno();
}
void MainWindow::on_actionDelete_selected_Annotation_triggered(){
	qobject_cast<ControllerMainWindow*> (getController())->emitDelSelAnno();
}



//////////////////////////////////Extras//////////////////////////////

void MainWindow::on_actionSettings_triggered() {
	m_SettingsWindow = new SettingsWindow();

	m_SettingsWindow->show();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    qobject_cast<ControllerMainWindow*> (getController())->exit();
}

void MainWindow::on_actionAbout_triggered() {
    std::string version = "";
    version = "BioTracker3<br><br>";

    version = "Version: ";
    version += GIT_HASH;
    version += +"<br><br>";

    version += "External libraries used:<br><br>";
    std::string qt = MyQT_VERSION;
    version += "QT " + qt + "<br>";

    std::string cv = MyCV_VERSION;
    version += "OpenCV " + cv + "<br>";

    std::string boost = MyBT_VERSION;
    version += "Boost " + boost + "<br>";

	version += "<br><br>Icons by <a href='https://icons8.com/'>icons8</a><br>";

    version += "<br><br>Published under LGPL licence";

	QMessageBox msgBox(this);
    msgBox.setWindowTitle("About BioTracker");
    msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
    msgBox.setText(tr(version.c_str()));
    msgBox.exec();

    //QMessageBox::about(this, tr("About BioTracker"), tr(version.c_str()));
}

void MainWindow::on_actionShortcuts_triggered() {

	//TODO import this from file
    std::pair<QString, QString> scUndo (QString("CTRL + Z"), QString("Undo"));
    std::pair<QString, QString> scRedo (QString("CTRL + Y"), QString("Redo"));
    std::pair<QString, QString> scSel (QString("CTRL + A"), QString("Select all"));
    std::pair<QString, QString> scPlay (QString("Space"), QString("Play"));
    std::pair<QString, QString> scNext (QString("Left Arrow"), QString("Previous Frame"));
    std::pair<QString, QString> scPrev (QString("Right Arrow"), QString("Next Frame"));
    std::pair<QString, QString> scStop (QString("CTRL + Space"), QString("Stop"));
    std::pair<QString, QString> scAL (QString("ALT + L"), QString("Add label annotation"));
    std::pair<QString, QString> scAA (QString("ALT + A"), QString("Add arrow annotation"));
    std::pair<QString, QString> scAE (QString("ALT + E"), QString("Add ellipse annotation"));
    std::pair<QString, QString> scAR (QString("ALT + R"), QString("Add rect annotation"));
    std::pair<QString, QString> scADel (QString("ALT + DELETE"), QString("Delete selected annotation"));

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
	outerWidget->setWindowTitle("Shortcuts");
	outerWidget->resize(scTable->size());
	QVBoxLayout* vLayout = new QVBoxLayout();

	vLayout->addWidget(scTable);

	outerWidget->setLayout(vLayout);


	outerWidget->show();


}




