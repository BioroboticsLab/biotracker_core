#include "BioTracker.h"

BioTracker::BioTracker(Settings &settings,QWidget *parent, Qt::WindowFlags flags) : 
	QMainWindow(parent, flags),
	_trackingThread(NULL),
	_settings(settings)
{
	ui.setupUi(this);
	init();
}

BioTracker::~BioTracker()
{

}

void BioTracker::init(){
	_videoPaused = false;
	_videoStopped = true;
	_currentFrame = 0;
	_trackingThread = new TrackingThread(_settings);
	_iconPause.addFile(QStringLiteral(":/BioTracker/pix/pause-sign.png"), QSize(), QIcon::Normal, QIcon::Off);
	_iconPlay.addFile(QStringLiteral(":/BioTracker/pix/arrow-forward1.png"), QSize(), QIcon::Normal, QIcon::Off);
	qRegisterMetaType<cv::Mat>("cv::Mat");
	initGui();
	initConnects();

}

void BioTracker::initGui()
{
	setPlayfieldEnabled(true);
	ui.actionOpen_Video->setEnabled(true);
}

void BioTracker::initConnects()
{
	//File -> Open Video
	QObject::connect(ui.actionOpen_Video, SIGNAL(triggered()), this, SLOT(browseVideo()));
	//video playfield buttons
	QObject::connect(ui.button_playPause, SIGNAL(clicked()), this, SLOT(runCapture()));
	QObject::connect(ui.button_stop, SIGNAL(clicked()), this, SLOT(stopCapture()));

	//tracking thread signals
	QObject::connect(this, SIGNAL(videoPause(bool)), _trackingThread, SLOT(enableVideoPause(bool)));
	QObject::connect(_trackingThread, SIGNAL( trackingSequenceDone(cv::Mat) ), this, SLOT( drawImage(cv::Mat) ));
	QObject::connect(_trackingThread, SIGNAL( newFrameNumber(int) ), this, SLOT( updateFrameNumber(int) ));
	QObject::connect(this, SIGNAL( nextFrameReady(bool) ), _trackingThread, SLOT( enableHandlingNextFrame(bool) ));
}

void BioTracker::browseVideo()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open video"), "", tr("video Files (*.avi *.wmv)"));
	if(filename.compare("") != 0){
		_settings.setParam(CAPTUREPARAM::CAP_VIDEO_FILE,filename.toStdString());
		setPlayfieldEnabled(true);
	}
	
}

void BioTracker::setPlayfieldEnabled(bool enabled){
	ui.button_playPause->setEnabled(enabled);
	ui.button_nextFrame->setEnabled(enabled);
	ui.button_previousFrame->setEnabled(enabled);
	ui.button_rewind->setEnabled(enabled);
	ui.button_stop->setEnabled(enabled);
	ui.button_screenshot->setEnabled(enabled);
}

void BioTracker::runCapture()
{	
	//check if video is stopped
	if (_videoStopped){
		_videoStopped = false;
		ui.button_playPause->setIcon(_iconPause);
		_trackingThread->startCapture();
	}
	
	else
	{	
		_videoPaused = !_videoPaused;
		if (_videoPaused)		
			ui.button_playPause->setIcon(_iconPlay);
		else
			ui.button_playPause->setIcon(_iconPause);
		
		emit videoPause(_videoPaused);
	}

	
}

void BioTracker::stepCapture()
{

}


void BioTracker::stopCapture()
{	
	_videoStopped = true;
	_videoPaused = false;
	emit videoPause(_videoPaused);
	ui.button_playPause->setIcon(_iconPlay);
	_trackingThread->stopCapture();

}

void BioTracker::updateFrameNumber(int frameNumber)
{
	_currentFrame = frameNumber;	
}

void BioTracker::drawImage(cv::Mat image)
{


	if(image.data)
		ui.videoView->showImage(image);

	// signals when the frame was drawn, and the FishTrackerThread can continue to work;
	emit nextFrameReady(true);
	
}