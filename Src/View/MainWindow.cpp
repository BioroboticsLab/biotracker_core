#include "MainWindow.h"

#include <QFileDialog>
#include <QLayout>
#include <QGraphicsObject>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QLinkedList>
#include <QMessageBox>
#include <QToolButton>
#include <QWizard>
#include <QWizardPage>
#include <QDesktopServices>
#include <QDesktopWidget>

#include "ui_MainWindow.h"

#include "util/types.h"

#include "Model/null_Model.h"
#include "View/CoreParameterView.h"
#include "View/GraphicsView.h"
#include "View/AnnotationsView.h"
#include "Controller/ControllerMainWindow.h"
#include "Controller/null_Controller.h"

#include "VideoControllWidget.h"

MainWindow::MainWindow(QWidget* parent, IController* controller, IModel* model) :
	IViewMainWindow(parent, controller, model),
	ui(new Ui::MainWindow) {
	_cfg = static_cast<IControllerCfg*>(controller)->getConfig();
	_previouslySelectedTracker = "";
	_currentParameterView = 0;
	_currentCoreParameterView = 0;
	_currentElementView = 0;
	ui->setupUi(this);

	//add cursor position label
	QLabel* _cursorPosition = new QLabel("0,0");
	_cursorPosition->setObjectName("_cursorPosition");
	statusBar()->insertPermanentWidget(0, _cursorPosition);

	//TODO
	ui->actionToggle_compact_menu_toolbar_2->setEnabled(false);

	//setup toolbars
	setupUpperToolBar();
	setupVideoToolBar();
	ui->toolBarTools->setEnabled(false);

	ui->toolBox->setStyleSheet("QToolBox::tab {"
		" background-color: #e5e5e5;}"
	);

	//resize to full size
	QWidget::showMaximized();

	createIntroductionWizard();

	//set window icon
	QApplication::setWindowIcon(QIcon(":/Logo/resources/logo/BT-3_non_transparent_rounded.ico"));

}

MainWindow::~MainWindow() {
	delete ui;
}

//******************************************SETUP*******************************************

void MainWindow::setupUpperToolBar() {

	_trackerActivator = new SwitchButton("not tracking", "tracking");
	_trackerActivator->setToolTip("Activate/Disable tracking");
	QObject::connect(_trackerActivator, &SwitchButton::emitSetEnabled, this, &MainWindow::receiveSetTracking, Qt::DirectConnection);
	//QAction* activatorAction = ui->toolBarMenu->addWidget(_trackerActivator);

	/////////////////////media groupbox
	_mediaBox = new QGroupBox("Load media");
	//style groupbox (initial red border -> no video loaded)
	_mediaBox->setStyleSheet("QGroupBox"
		" {border: 1px solid #e56262;}");
	// "QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top center; padding: 0 3px;}");
	QFont font = QFont();
	font.setPointSize(8);
	font.setBold(true);
	_mediaBox->setFont(font);

	QHBoxLayout* mediaBoxLayout = new QHBoxLayout;
	mediaBoxLayout->setContentsMargins(0, 0, 0, 0);
	//mediaBoxLayout->setSpacing(0);
	QToolButton* cameraButton = new QToolButton;
	cameraButton->setIconSize(QSize(24, 24));
	cameraButton->setDefaultAction(ui->actionOpen_Camera);
	cameraButton->setAutoRaise(true);

	QToolButton* videoButton = new QToolButton;
	videoButton->setIconSize(QSize(24, 24));
	videoButton->setDefaultAction(ui->actionOpen_Video);
	videoButton->setAutoRaise(true);

	QToolButton* imageButton = new QToolButton;
	imageButton->setIconSize(QSize(24, 24));
	imageButton->setDefaultAction(ui->actionOpen_Picture);
	imageButton->setAutoRaise(true);

	mediaBoxLayout->addWidget(videoButton);
	mediaBoxLayout->addWidget(imageButton);
	mediaBoxLayout->addWidget(cameraButton);

	_mediaBox->setLayout(mediaBoxLayout);


	ui->toolBarMenu->addWidget(_mediaBox);

	////////trackerbox
	_trackerBox = new QGroupBox("Load tracker");
	//style groupbox
	_trackerBox->setStyleSheet("QGroupBox"
		" {border: 1px solid #e56262;}");
	// "QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top center; padding: 0 3px;}");

	QFont font0 = QFont();
	font0.setPointSize(8);
	font0.setBold(true);
	_trackerBox->setFont(font0);

	QHBoxLayout* trackerBoxLayout = new QHBoxLayout;
	trackerBoxLayout->setContentsMargins(0, 0, 0, 0);

	QToolButton* trackerButton = new QToolButton;
	trackerButton->setIconSize(QSize(24, 24));
	trackerButton->setDefaultAction(ui->actionLoad_Tracker);
	trackerButton->setAutoRaise(true);

	trackerBoxLayout->addWidget(trackerButton);
	_trackerBox->setLayout(trackerBoxLayout);

	ui->toolBarMenu->addWidget(_trackerBox);


	////////loadSaveBox
	QGroupBox* loadSaveBox = new QGroupBox("Load/Save Data");
	//style loadSaveBox
	// loadSaveBox->setStyleSheet("QGroupBox"
	// " {border: 1px solid #e5e5e5;border-radius: 5px;margin-top: 1ex; /* leave space at the top for the title */}"
	// "QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top center; padding: 0 3px;}");

	QFont font2 = QFont();
	font2.setPointSize(8);
	font2.setBold(true);
	loadSaveBox->setFont(font2);

	QHBoxLayout* loadSaveBoxLayout = new QHBoxLayout;
	loadSaveBoxLayout->setContentsMargins(5, 0, 5, 0);

	QToolButton* loadFileButton = new QToolButton;
	loadFileButton->setIconSize(QSize(24, 24));
	loadFileButton->setDefaultAction(ui->actionLoad_trackingdata);
	loadFileButton->setAutoRaise(true);

	QToolButton* saveFileButton = new QToolButton;
	saveFileButton->setIconSize(QSize(24, 24));
	saveFileButton->setDefaultAction(ui->actionSave_trackingdata);
	saveFileButton->setAutoRaise(true);

	loadSaveBoxLayout->addWidget(loadFileButton);
	loadSaveBoxLayout->addWidget(saveFileButton);
	loadSaveBox->setLayout(loadSaveBoxLayout);

	ui->toolBarMenu->addWidget(loadSaveBox);

	//////////////////choose tracker groupbox
	QGroupBox* chooseTrackerBox = new QGroupBox("Choose tracker");
	//style groupbox
	// chooseTrackerBox->setStyleSheet("QGroupBox"
	// " {border: 1px solid #e5e5e5;border-radius: 5px;margin-top: 1ex; /* leave space at the top for the title */}"
	// "QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top center; padding: 0 3px;}");

	QFont font1 = QFont();
	font1.setPointSize(8);
	font1.setBold(true);
	chooseTrackerBox->setFont(font1);
	//
	QHBoxLayout* chooseTrackerBoxLayout = new QHBoxLayout;
	chooseTrackerBoxLayout->addWidget(ui->comboBox_TrackerSelect);
	chooseTrackerBoxLayout->addWidget(_trackerActivator);

	chooseTrackerBox->setLayout(chooseTrackerBoxLayout);

	ui->toolBarMenu->addWidget(chooseTrackerBox);


}

void MainWindow::checkTrackerGroupBox() {
	_trackerBox->setStyleSheet("QGroupBox"
		" {border: 1px solid #82c985;}");
}

void MainWindow::checkMediaGroupBox() {
	_mediaBox->setStyleSheet("QGroupBox"
		" {border: 1px solid #82c985;}");
}

void MainWindow::setupVideoToolBar() {

}


void MainWindow::addVideoControllWidget(IView* widget) {
	QLayout* layout = new QGridLayout();
	//layout->addWidget(dynamic_cast<QWidget*>(widget));

	QWidget* controlW = dynamic_cast<QWidget*>(widget);
	if (controlW) {
		layout->addWidget(controlW->findChild<QSlider *>("sld_video"));
	}
	ui->videoControls->setLayout(layout);

	//ui->videoControls->adjustSize();

}

void MainWindow::addVideoView(IView* videoView) {
	m_graphView = dynamic_cast<GraphicsView*>(videoView);
	m_graphView->setParent(ui->trackingArea);

	ui->videoViewLayout->addWidget(m_graphView);
}

void MainWindow::addTrackerElementsView(IView *elemView)
{

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

		ui->notificationTab->layout()->addWidget(notificationWidget);

		notificationWidget->setVisible(1);
	}

	//createIntroductionWizard();
}

void MainWindow::addTrackerParameterView(IView *parameter)
{
	QWidget* pluginParameter = dynamic_cast<QWidget*>(parameter);


	while (auto item = ui->trackerOptions->layout()->takeAt(0)) {
		delete item->widget();
	}

	ui->trackerOptions->layout()->addWidget(pluginParameter);

}

void MainWindow::addCoreParameterView(IView * coreParameterView)
{

	QWidget* coreParameter = dynamic_cast<QWidget*>(coreParameterView);
	if (coreParameter) {
		//add experiment widget to toolbar
		QWidget* experimentWidget = coreParameter->findChild<QWidget*>("experimentWidget");
		if (experimentWidget) {
			ui->experimentItem->layout()->addWidget(experimentWidget);
		}


		ui->widget_alg->updateGeometry();
		coreParameter->updateGeometry();
		coreParameter->setContentsMargins(QMargins(0, 0, 0, 0));
		coreParameter->setParent(ui->widget_alg);

		QHBoxLayout* hLayout = new QHBoxLayout;
		hLayout->addWidget(coreParameter, 100, 0);
		hLayout->setContentsMargins(QMargins(0, 0, 0, 0)); //left, top, right, bottom
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

void MainWindow::setTrackerList(QStringListModel* trackerList, QString current) {
	ui->comboBox_TrackerSelect->setModel(trackerList);
	ui->comboBox_TrackerSelect->setCurrentText(current);
}

//TODO put this in a class
void MainWindow::createIntroductionWizard() {
	m_introWiz = new QWizard;

	//BioTracker::Core::Settings *disableIntroWiz = GET_CORESETTINGS(CORE_CONFIGURATION);
	bool disabled = _cfg->DisableWizard;

	//dont show again checkbox
	QCheckBox* noShowCheck = new QCheckBox("Don't show this again");
	noShowCheck->setChecked(disabled);
	QObject::connect(noShowCheck, &QCheckBox::toggled, this, &MainWindow::toggleNoShowWiz);


	//introduction
	QWizardPage* p1 = new QWizardPage;
	{
		p1->setTitle("BioTracker 3 - Introduction");

		QPixmap logoImg(":/Logo/resources/logo/BT3-big.png");
		QPixmap scaledImg = logoImg.scaled(QSize(600, 400), Qt::KeepAspectRatio, Qt::SmoothTransformation);

		QLabel* imgLabel = new QLabel;
		imgLabel->setPixmap(scaledImg);
		imgLabel->setAttribute(Qt::WA_TranslucentBackground);
		imgLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		imgLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		QFile file(":/Introduction/resources/introduction/intro.txt");
		QLabel *introLabel = new QLabel;
		introLabel->setWordWrap(true);

		//read introduction text from file
		QString line;
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream stream(&file);
			while (!stream.atEnd()) {
				line.append(stream.readLine() + "\n");
			}
			introLabel->setText(line);
		}
		file.close();

		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(imgLabel);
		QScrollArea* textScroll = new QScrollArea;
		textScroll->setFrameShape(QFrame::NoFrame);
		textScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		textScroll->setWidget(introLabel);
		textScroll->setWidgetResizable(true);
		layout->addWidget(textScroll);
		layout->setAlignment(Qt::AlignHCenter);
		p1->setLayout(layout);

	}

	//overview
	QWizardPage* p2 = new QWizardPage;
	{
		p2->setTitle("BioTracker 3 - Overview");
		QLabel *label = new QLabel("This is the BioTracker. Don't let yourself be overwhelmed as it is actually pretty simple.");
		label->setWordWrap(true);

		QPixmap overviewImg(":/Introduction/resources/introduction/images/BioTracker.PNG");
		QPixmap scaledImg = overviewImg.scaled(QSize(600, 400), Qt::KeepAspectRatio, Qt::SmoothTransformation);

		QLabel* imgLabel = new QLabel;
		imgLabel->setPixmap(scaledImg);
		imgLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		//imgLabel->setScaledContents(true);

		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(imgLabel);
		QScrollArea* textScroll = new QScrollArea;
		textScroll->setFrameShape(QFrame::NoFrame);
		textScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		textScroll->setWidget(label);
		textScroll->setWidgetResizable(true);
		layout->addWidget(textScroll);
		layout->setAlignment(Qt::AlignHCenter);
		p2->setLayout(layout);
	}

	//toolbars
	QWizardPage* p3 = new QWizardPage;
	{
		p3->setTitle("BioTracker 3 - Toolbars");

		//image
		QPixmap overviewImg(":/Introduction/resources/introduction/images/toolbars.png");
		QPixmap scaledImg = overviewImg.scaled(QSize(600, 400), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QLabel* imgLabel = new QLabel;

		imgLabel->setPixmap(scaledImg);

		//read toolbar text from file
		QFile file(":/Introduction/resources/introduction/toolbars.txt");
		QLabel *label = new QLabel();
		label->setWordWrap(true);

		QString line;
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream stream(&file);
			while (!stream.atEnd()) {
				line.append(stream.readLine() + "\n");
			}
			label->setText(line);
		}
		file.close();


		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(imgLabel);
		QScrollArea* textScroll = new QScrollArea;
		textScroll->setFrameShape(QFrame::NoFrame);
		textScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		textScroll->setWidget(label);
		textScroll->setWidgetResizable(true);
		layout->addWidget(textScroll);
		layout->setAlignment(Qt::AlignHCenter);
		p3->setLayout(layout);
	}


	//right panel - overview
	QWizardPage* p4 = new QWizardPage;
	{
		p4->setTitle("BioTracker 3 - Right panel overview");

		//image
		QPixmap overviewImg(":/Introduction/resources/introduction/images/right_panel.png");
		QPixmap scaledImg = overviewImg.scaled(QSize(600, 400), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QLabel* imgLabel = new QLabel;
		imgLabel->setPixmap(scaledImg);

		//read right panel text from file
		QFile file(":/Introduction/resources/introduction/right_panel.txt");
		QLabel *label = new QLabel();
		label->setWordWrap(true);

		QString line;
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream stream(&file);
			while (!stream.atEnd()) {
				line.append(stream.readLine() + "\n");
			}
			label->setText(line);
		}
		file.close();


		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(imgLabel);
		QScrollArea* textScroll = new QScrollArea;
		textScroll->setFrameShape(QFrame::NoFrame);
		textScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		textScroll->setWidget(label);
		textScroll->setWidgetResizable(true);
		layout->addWidget(textScroll);
		layout->setAlignment(Qt::AlignHCenter);

		p4->setLayout(layout);
	}


	//how to track
	QWizardPage* p5 = new QWizardPage;
	{
		p5->setTitle("BioTracker 3 - Tracking");

		//image
		QPixmap overviewImg(":/Introduction/resources/introduction/images/tracking.png");
		QPixmap scaledImg = overviewImg.scaled(QSize(600, 400), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QLabel* imgLabel = new QLabel;
		imgLabel->setPixmap(scaledImg);

		//read tracking text from file
		QFile file(":/Introduction/resources/introduction/tracking.txt");
		QLabel *label = new QLabel();
		label->setWordWrap(true);

		QString line;
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream stream(&file);
			while (!stream.atEnd()) {
				line.append(stream.readLine() + "\n");
			}
			label->setText(line);
		}
		file.close();


		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(imgLabel);
		QScrollArea* textScroll = new QScrollArea;
		textScroll->setFrameShape(QFrame::NoFrame);
		textScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		textScroll->setWidget(label);
		textScroll->setWidgetResizable(true);
		layout->addWidget(textScroll);
		layout->setAlignment(Qt::AlignHCenter);

		p5->setLayout(layout);
	}

	//context menus
	QWizardPage* p6 = new QWizardPage;
	{
		p6->setTitle("BioTracker 3 - Context menus");

		//images
		QPixmap contextEntityImg(":/Introduction/resources/introduction/images/context_entity.png");
		QPixmap scaledImg1 = contextEntityImg.scaled(QSize(300, 400), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QLabel* imgEntityLabel = new QLabel;
		imgEntityLabel->setPixmap(scaledImg1);

		QPixmap contextVideoImg(":/Introduction/resources/introduction/images/context_video.png");
		QPixmap scaledImg2 = contextVideoImg.scaled(QSize(300, 400), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QLabel* imgVideoLabel = new QLabel;
		imgVideoLabel->setPixmap(scaledImg2);

		//read tracking text from file
		QFile file(":/Introduction/resources/introduction/context_menus.txt");
		QLabel *label = new QLabel();
		label->setWordWrap(true);

		QString line;
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream stream(&file);
			while (!stream.atEnd()) {
				line.append(stream.readLine() + "\n");
			}
			label->setText(line);
		}
		file.close();


		QVBoxLayout *outerLayout = new QVBoxLayout;
		QHBoxLayout *imgLayout = new QHBoxLayout;
		imgLayout->addWidget(imgEntityLabel);
		imgLayout->addWidget(imgVideoLabel);
		QFrame* imgFrame = new QFrame;
		imgFrame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
		imgFrame->setLayout(imgLayout);

		outerLayout->addWidget(imgFrame);
		QScrollArea* textScroll = new QScrollArea;
		textScroll->setFrameShape(QFrame::NoFrame);
		textScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		textScroll->setWidget(label);
		textScroll->setWidgetResizable(true);
		outerLayout->addWidget(textScroll);
		outerLayout->addWidget(noShowCheck);

		outerLayout->setAlignment(Qt::AlignHCenter);

		p6->setLayout(outerLayout);
	}


	m_introWiz->addPage(p1);
	m_introWiz->addPage(p2);
	m_introWiz->addPage(p3);
	m_introWiz->addPage(p4);
	m_introWiz->addPage(p5);
	m_introWiz->addPage(p6);
	//m_introWiz->addPage(p7);


	m_introWiz->setWindowTitle("Introduction");
	if (!disabled) {
		QRect rec = QApplication::desktop()->availableGeometry();
		m_introWiz->resize(m_introWiz->width(), 500);
		m_introWiz->show();
		//m_introWiz->showMaximized();
	}
}

///////////////////////////////////////////

void MainWindow::setCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool> permission) {
	if (permission.first == ENUMS::COREPERMISSIONS::COMPONENTADD) {
		ui->actionAdd_Track->setEnabled(permission.second);
		return;
	}
	if (permission.first == ENUMS::COREPERMISSIONS::COMPONENTREMOVE) {
		ui->actionDelete_selected_tracks->setEnabled(permission.second);
		return;
	}
	if (permission.first == ENUMS::COREPERMISSIONS::COMPONENTSWAP) {
		ui->actionSwap_ID_s->setEnabled(permission.second);
		return;
	}
}

void MainWindow::on_comboBox_TrackerSelect_currentIndexChanged(QString s) {
	QString ct = ui->comboBox_TrackerSelect->currentText();
	if (!ct.isEmpty() && _previouslySelectedTracker != ct) {
		_previouslySelectedTracker = ct;
		Q_EMIT selectPlugin(ct);
	}
}

void MainWindow::setCursorPositionLabel(QPoint pos)
{
	QString posString = QString("%1, %2").arg(QString::number(pos.x()), QString::number(pos.y()));


	QLabel* currentChild = statusBar()->findChild<QLabel*>("_cursorPosition");
	currentChild->setText(posString);

}

void MainWindow::activeTrackingCheckBox() {
	//ui->checkBox_TrackingActivated->setEnabled(true);
	_trackerActivator->setEnabled(true);
	checkTrackerGroupBox();
	ui->toolBarTools->setEnabled(true);
}

void MainWindow::deactivateTrackingCheckBox() {
	//ui->checkBox_TrackingActivated->setEnabled(false);
	_trackerActivator->setEnabled(false);
}

void MainWindow::saveDataToFile() {
	static const QString fileFilter(
		"tracking data files (*.csv *.dat *.json)");

	boost::filesystem::path file;

	std::string locStr = _cfg->DefaultLocationManualSave.toStdString();
	QString locStrQ = QString::fromStdString(locStr);

	QString f = QFileDialog::getSaveFileName(this, "Save trajectory data to file", locStrQ, fileFilter, 0);

	if (f != "") {
		qobject_cast<ControllerMainWindow*> (getController())->saveTrajectoryFile(f.toStdString());
	}
}

////////////////////////////////////////////////SLOTS/////////////////////////////////////

void MainWindow::toggleNoShowWiz(bool toggle) {
	_cfg->DisableWizard = toggle;//Todo implicit cast bool->int
}

void MainWindow::receiveSelectedCameraDevice(CameraConfiguration conf) {
	qobject_cast<ControllerMainWindow*> (getController())->loadCameraDevice(conf);

}

void MainWindow::on_rightPanelViewControllerButton_clicked() {
	QList<int> splitterSizes = QList<int>();
	if (ui->widgetParameterAreaOuterCanvas->isVisible()) {
		_lastRightPanelWidth = ui->mainSplitter->sizes()[1]; //save last width to restore on show
		_lastVideoWidgetWidth = ui->mainSplitter->sizes()[0];

		ui->widgetParameterAreaOuterCanvas->hide();
		splitterSizes << 20000 << ui->rightPanelViewControllerWidget->minimumSizeHint().width();

		ui->rightPanelViewControllerButton->setText("<");
		//set menu action unchecked
		ui->actionRight_panel->setChecked(false);
	}
	else {
		ui->widgetParameterAreaOuterCanvas->show();
		splitterSizes << _lastVideoWidgetWidth << _lastRightPanelWidth;
		ui->rightPanelViewControllerButton->setText(">");

		//set menu action checked
		ui->actionRight_panel->setChecked(true);
	}
	ui->mainSplitter->setSizes(splitterSizes);
}

void MainWindow::on_bottomPanelViewControllerButton_clicked() {
	ui->videoControls->setVisible(ui->bottomPanelViewControllerButton->text() == "^");
	ui->bottomPanelViewControllerButton->setText(ui->bottomPanelViewControllerButton->text() == "v" ? "^" : "v");
}

void MainWindow::activateTracking() {
	_trackerActivator->setState(true);
}

void MainWindow::deactivateTracking() {
	_trackerActivator->setState(false);
}

void MainWindow::receiveSetTracking(bool toggle) {
	if (toggle) {
		qobject_cast<ControllerMainWindow*> (getController())->activeTracking();
	}
	else {
		qobject_cast<ControllerMainWindow*> (getController())->deactiveTrackring();
	}
}

void MainWindow::resetTrackerViews() {
	if (_currentElementView && _currentCoreView) {
		_currentElementView->setParent(0);
		_currentCoreView->setParent(0);
		m_graphView->removeGraphicsItem(_currentElementView);
		m_graphView->removeGraphicsItem(_currentCoreView);
	}

	_currentElementView = nullptr;
	_currentCoreView = nullptr;
}

//////////////////////////utility toolbar action slots///////////////////////////
void MainWindow::on_actionAdd_Track_triggered() {
	qobject_cast<ControllerMainWindow*> (getController())->emitAddTrack();
}
void MainWindow::on_actionDelete_selected_tracks_triggered() {
	qobject_cast<ControllerMainWindow*> (getController())->emitDeleteSelectedTracks();
}
void MainWindow::on_actionSwap_ID_s_triggered() {
	qobject_cast<ControllerMainWindow*> (getController())->emitSwapIds();

}
void MainWindow::on_actionSelect_all_triggered() {
	qobject_cast<ControllerMainWindow*> (getController())->emitSelectAll();
}
void MainWindow::on_actionChange_the_border_color_triggered() {
	qobject_cast<ControllerMainWindow*> (getController())->emitChangeColorBorder();
}
void MainWindow::on_actionChange_the_fill_color_triggered() {
	qobject_cast<ControllerMainWindow*> (getController())->emitChangeColorFill();
}
void MainWindow::on_actionAdd_label_Annotation_triggered() {
	qobject_cast<ControllerMainWindow*> (getController())->emitAddLabelAnno();
}
void MainWindow::on_actionAdd_rectangular_annotation_triggered() {
	qobject_cast<ControllerMainWindow*> (getController())->emitAddRectAnno();
}
void MainWindow::on_actionAdd_arrow_annotation_triggered() {
	qobject_cast<ControllerMainWindow*> (getController())->emitAddArrAnno();
}
void MainWindow::on_actionAdd_elliptical_annotation_triggered() {
	qobject_cast<ControllerMainWindow*> (getController())->emitAddEllAnno();
}
void MainWindow::on_actionDelete_selected_Annotation_triggered() {
	qobject_cast<ControllerMainWindow*> (getController())->emitDelSelAnno();
}


///////////////////////////////menu->file slots/////////////////////////////

void MainWindow::on_actionOpen_Video_triggered() {
	static const QString videoFilter("Video files (*.avi *.wmv *.mp4 *.mkv *.mov)");

	QString filename = QFileDialog::getOpenFileName(this,
		"Open video", "", videoFilter, 0);

	if (!filename.isEmpty()) {
		dynamic_cast<ControllerMainWindow*> (getController())->loadVideo({filename.toStdString().c_str()});
	}
}

void MainWindow::on_actionOpen_Video_batch_triggered() {
    static const QString videoFilter("Video files (*.avi *.wmv *.mp4 *.mkv *.mov)");

    std::vector<boost::filesystem::path> files;

    for (QString const& path : QFileDialog::getOpenFileNames(this,
        "Open video files", "", videoFilter, 0)) {
        files.push_back(boost::filesystem::path(path.toStdString()));
    }

    if (!files.empty()) {
        qobject_cast<ControllerMainWindow*> (getController())->loadVideo(files);
    }
}

void MainWindow::on_actionLoad_Tracker_triggered() {
	static const QString pluginFilter("BioTracker Tracking Plugin files (*.bio_tracker.so *.bio_tracker.dll *.bio_tracker.dylib)");

	QString filename = QFileDialog::getOpenFileName(this,
		"Open BioTracker Tracking Plugin", "", pluginFilter, 0);

	if (!filename.isEmpty()) {
		qobject_cast<ControllerMainWindow*> (getController())->loadTracker(filename);
	}
}

void MainWindow::on_actionOpen_Picture_triggered() {
	static const QString imageFilter(
		"image files (*.png *.jpg *.jpeg *.gif *.bmp *.jpe *.ppm *.tiff *.tif *.sr *.ras *.pbm *.pgm *.jp2 *.dib)");

	std::vector<boost::filesystem::path> files;
	for (QString const& path : QFileDialog::getOpenFileNames(this,
		"Open image files", "", imageFilter, 0)) {
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
		"Open tracking file", "", imageFilter, 0)) {
		files.push_back(boost::filesystem::path(path.toStdString()));
	}

	if (!files.empty()) {
		qobject_cast<ControllerMainWindow*> (getController())->loadTrajectoryFile(files[0].string());
	}
}

void MainWindow::on_actionSave_trackingdata_triggered() {
	saveDataToFile();
}

void MainWindow::on_actionOpen_Camera_triggered() {
	m_CameraDevice = new CameraDevice();

	m_CameraDevice->show();

	QObject::connect(m_CameraDevice, &CameraDevice::emitSelectedCameraDevice, this, &MainWindow::receiveSelectedCameraDevice);
}

void MainWindow::on_actionQuit_triggered() {
	qobject_cast<ControllerMainWindow*> (getController())->exit();
}

///////////////////////////////menu->edit slots/////////////////////////////

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

void MainWindow::on_actionSettings_triggered() {
	m_SettingsWindow = new SettingsWindow(0, _cfg);

	m_SettingsWindow->show();
}

///////////////////////////////menu->view slots/////////////////////////////

//toggle toolbars actions
void MainWindow::on_actionToggle_menu_toolbar_triggered() {
	bool currentState = ui->toolBarMenu->isVisible();
	ui->toolBarMenu->setVisible(!currentState);
	ui->actionToggle_menu_toolbar->setText(!currentState ? "Hide menu toolbar" : "Show menu toolbar");
}
void MainWindow::on_actionToggle_view_toolbar_triggered() {
	bool currentState = ui->toolBarTools->isVisible();
	ui->toolBarTools->setVisible(!currentState);
	ui->actionToggle_view_toolbar->setText(!currentState ? "Hide view toolbar" : "Show view toolbar");
}
void MainWindow::on_actionToggle_video_toolbar_triggered() {
	bool currentState = ui->toolBarVideo->isVisible();
	ui->toolBarVideo->setVisible(!currentState);
	ui->actionToggle_video_toolbar->setText(!currentState ? "Hide view toolbar" : "Show view toolbar");
}

void MainWindow::on_actionToggle_compact_menu_toolbar_2_triggered() {
	ui->toolBarMenu->setStyleSheet("QGroupBox"
		" {border-style: none; margin-top: 0ex; font-size: 1px;}"
		"QGroupBox::title {font-size: 1px; padding: 0 3px;}");


}

void MainWindow::on_toolBarMenu_visibilityChanged(bool visible) {
	ui->actionToggle_menu_toolbar->setText(visible ? "Hide menu toolbar" : "Show menu toolbar");
}
void MainWindow::on_toolBarTools_visibilityChanged(bool visible) {
	ui->actionToggle_view_toolbar->setText(visible ? "Hide view toolbar" : "Show view toolbar");
}
void MainWindow::on_toolBarVideo_visibilityChanged(bool visible) {
	ui->actionToggle_video_toolbar->setText(visible ? "Hide video toolbar" : "Show video toolbar");
}

void MainWindow::on_actionBottom_panel_triggered(bool checked) {
	ui->bottomPanelViewControllerButton->click();
}
void MainWindow::on_actionRight_panel_triggered(bool checked) {
	ui->rightPanelViewControllerButton->click();
}

void MainWindow::on_actionToggle_fullscreen_triggered() {
	if (isFullScreen()) {
		showNormal();
	}
	else {
		showFullScreen();
	}
}

//////////////////////////////////menu->Go to slots//////////////////////////////////

void MainWindow::on_actionOpen_Plugins_directory_triggered() {
	QDesktopServices::openUrl(QUrl::fromLocalFile(_cfg->DirPlugins));
}
void MainWindow::on_actionOpen_Track_directory_triggered() {
	QDesktopServices::openUrl(QUrl::fromLocalFile(_cfg->DirTracks));
}
void MainWindow::on_actionOpen_Trial_directory_triggered() {
	QDesktopServices::openUrl(QUrl::fromLocalFile(_cfg->DirTrials));
}
void MainWindow::on_actionOpen_Screenshot_directory_triggered() {
	QDesktopServices::openUrl(QUrl::fromLocalFile(_cfg->DirScreenshots));
}
void MainWindow::on_actionOpen_Videos_directory_triggered() {
	QDesktopServices::openUrl(QUrl::fromLocalFile(_cfg->DirVideos));
}


//////////////////////////////////menu->Help slots//////////////////////////////

void MainWindow::on_actionUser_guide_triggered() {
	QDesktopServices::openUrl(QUrl("https://github.com/BioroboticsLab/biotracker_core/wiki"));
}

void MainWindow::on_actionAbout_triggered() {
	std::string version = "";
	version = "BioTracker3<br><br>";

	version = "Source Version: ";
	version += SOURCE_VERSION;
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

	QTableWidget* scTable = new QTableWidget();

	scTable->setRowCount(0);
	scTable->setColumnCount(2);

	scTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Shortcut"));
	scTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Description"));
	scTable->verticalHeader()->hide();

	QLinkedList<QPair<QString, QString>> scList;

	//read from file in resources and add to linked list
	QFile shortcutFile(":Shortcuts/resources/shortcuts.txt");

	if (!shortcutFile.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QString line;
	while (!shortcutFile.atEnd()) {
		QTextStream stream(&shortcutFile);
		while (!stream.atEnd()) {
			line = stream.readLine();
			QStringList shortCutAndDescription = line.split(':');
			if (shortCutAndDescription.size() == 2) {
				scList.append(QPair<QString, QString>(shortCutAndDescription[0], shortCutAndDescription[1]));
			}
		}

	}

	QLinkedList<QPair<QString, QString>>::const_iterator sc;
	for (sc = scList.constBegin(); sc != scList.constEnd(); ++sc) {
		scTable->insertRow(scTable->rowCount());


		QTableWidgetItem* scKey = new QTableWidgetItem(sc->first);
		QTableWidgetItem* scKeyInfo = new QTableWidgetItem(sc->second);
		scKey->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		scKeyInfo->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

		scTable->setItem(scTable->rowCount() - 1, 0, scKey);
		scTable->setItem(scTable->rowCount() - 1, 1, scKeyInfo);
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

void MainWindow::on_actionShow_introduction_triggered() {
	m_introWiz->show();
}

/////////////
void MainWindow::closeEvent(QCloseEvent *event) {
	qobject_cast<ControllerMainWindow*> (getController())->exit();
}