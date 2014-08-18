#include "BioTracker.h"

BioTracker::BioTracker(Settings &settings,QWidget *parent, Qt::WindowFlags flags) : 
	QMainWindow(parent, flags),
	_trackingThread(NULL),
	_settings(settings)
{
	ui.setupUi(this);
	init();
}


void BioTracker::init(){
	_videoPaused = true;
	_videoStopped = true;
	_currentFrame = 0;
	_trackingThread = new TrackingThread(_settings);
	_iconPause.addFile(QStringLiteral(":/BioTracker/pix/pause-sign.png"), QSize(), QIcon::Normal, QIcon::Off);
	_iconPlay.addFile(QStringLiteral(":/BioTracker/pix/arrow-forward1.png"), QSize(), QIcon::Normal, QIcon::Off);
	//meta types
	qRegisterMetaType<cv::Mat>("cv::Mat");
	qRegisterMetaType<MSGS::MTYPE>("MSGS::MTYPE");
	qRegisterMetaType<std::string>("std::string");
	initGui();
	initConnects();

}

void BioTracker::initGui()
{
	setPlayfieldPaused(true);
	ui.actionOpen_Video->setEnabled(true);
}

void BioTracker::initConnects()
{
	//File -> Open Video
	QObject::connect(ui.actionOpen_Video, SIGNAL(triggered()), this, SLOT(browseVideo()));
	//video playfield buttons
	QObject::connect(ui.button_playPause, SIGNAL(clicked()), this, SLOT(runCapture()));
	QObject::connect(ui.button_stop, SIGNAL(clicked()), this, SLOT(stopCapture()));
	QObject::connect(ui.button_nextFrame, SIGNAL( clicked() ), this, SLOT(stepCaptureForward())); 
	QObject::connect(ui.button_previousFrame, SIGNAL( clicked() ), this, SLOT(stepCaptureBackward())); 

	//slider
	QObject::connect(ui.sld_video, SIGNAL(sliderPressed()),this, SLOT(pauseCapture()));
	QObject::connect(ui.sld_video, SIGNAL( sliderMoved(int) ), this, SLOT( changeCurrentFrameBySlider(int)));

	//tracking thread signals
	QObject::connect(_trackingThread, SIGNAL(notifyGUI(std::string, MSGS::MTYPE)), this, SLOT(printGuiMessage(std::string, MSGS::MTYPE)));
	QObject::connect(this, SIGNAL(videoPause(bool)), _trackingThread, SLOT(enableVideoPause(bool)));
	QObject::connect(_trackingThread, SIGNAL( trackingSequenceDone(cv::Mat) ), this, SLOT( drawImage(cv::Mat) ));
	QObject::connect(_trackingThread, SIGNAL( newFrameNumber(int) ), this, SLOT( updateFrameNumber(int) ));
	QObject::connect(this, SIGNAL( nextFrameReady(bool) ), _trackingThread, SLOT( enableHandlingNextFrame(bool) ));
	QObject::connect(this, SIGNAL( changeFrame(int) ), _trackingThread, SLOT( setFrameNumber(int)));
	QObject::connect(this, SIGNAL( grabNextFrame()), _trackingThread, SLOT( nextFrame()));
}

void BioTracker::browseVideo()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open video"), "", tr("video Files (*.avi *.wmv)"));
	if(filename.compare("") != 0){
		_settings.setParam(CAPTUREPARAM::CAP_VIDEO_FILE,filename.toStdString());
		setPlayfieldPaused(true);
	}
	
}

void BioTracker::setPlayfieldPaused(bool enabled){
	switch(enabled)
	{
		//video is paused
		case true:			
			ui.button_nextFrame->setEnabled(true);
			ui.button_previousFrame->setEnabled(true);						
			break;
		//video is playing
		case false:			
			ui.button_nextFrame->setEnabled(false);
			ui.button_previousFrame->setEnabled(false);						
			break;
	}
}

void BioTracker::runCapture()
{	
	//check if video is stopped
	if (_videoStopped){
		_videoStopped = false;
		_videoPaused = false;
		ui.button_playPause->setIcon(_iconPause);
		_trackingThread->startCapture();
		ui.sld_video->setMaximum(_trackingThread->getVideoLength());
		ui.sld_video->setEnabled(true);
		setPlayfieldPaused(false);
	}
	
	else
	{	
		_videoPaused = !_videoPaused;
		if (_videoPaused){		
			ui.button_playPause->setIcon(_iconPlay);
			setPlayfieldPaused(true);
		}
		else
		{
			ui.button_playPause->setIcon(_iconPause);
			setPlayfieldPaused(false);
		}
		emit videoPause(_videoPaused);
	}

	
}

void BioTracker::stepCaptureForward()
{
	//if video not yet loaded, load it now!
	if(_videoStopped){
		_videoStopped = false;
		emit videoPause(true);
		_trackingThread->startCapture();
	}
	_videoPaused = true;
	emit grabNextFrame();
}

void BioTracker::stepCaptureBackward()
{
	if(_currentFrame > 0)
	{
		_currentFrame -= 1;
		emit changeFrame(_currentFrame);
		ui.sld_video->setValue(_currentFrame);
	}
}

void BioTracker::pauseCapture()
{
	emit videoPause(true);
	_videoPaused = true;
	ui.button_playPause->setIcon(_iconPlay);
}


void BioTracker::stopCapture()
{	
	_videoStopped = true;
	_videoPaused = true;
	ui.button_playPause->setIcon(_iconPlay);
	_trackingThread->stopCapture();
	setPlayfieldPaused(true);
	_currentFrame=0;
	ui.sld_video->setValue(_currentFrame);
	emit videoPause(false);

}

void BioTracker::updateFrameNumber(int frameNumber)
{
	_currentFrame = frameNumber;
	ui.sld_video->setValue(_currentFrame);
}

void BioTracker::drawImage(cv::Mat image)
{


	if(image.data)
		ui.videoView->showImage(image);

	// signals when the frame was drawn, and the FishTrackerThread can continue to work;
	emit nextFrameReady(true);
	
}

void BioTracker::printGuiMessage(std::string message, MSGS::MTYPE mType)
{
	QString msgLine =  "<span style=\"color:blue\">";	
	msgLine += QDateTime::currentDateTime().toString("hh:mm:ss");
	msgLine += "</span>&nbsp;&nbsp;&nbsp;";
	switch (mType)
	{
	case MSGS::MTYPE::WARNING:
		msgLine += "<span style=\"color:red\"><b> Warning: </b></span>";
		break;
	case MSGS::MTYPE::FAIL:
		msgLine += "<span style=\"color:red\"><b> Error: </b></span>";
	default:
		msgLine += " ";
		break;
	}
	msgLine.append(StringHelper::toQString(message));
	ui.edit_notification->append(msgLine);
}

void BioTracker::changeCurrentFrameBySlider(int value)
{	
	emit changeFrame(value);	
}