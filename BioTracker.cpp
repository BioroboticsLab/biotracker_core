#include "BioTracker.h"

BioTracker::BioTracker(Settings &settings,QWidget *parent, Qt::WindowFlags flags) : 
	QMainWindow(parent, flags),
	_trackingThread(NULL),
	_settings(settings)
{
	ui.setupUi(this);
	init();
}

//function to test file existence
inline bool file_exist(const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
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
	ui.sld_video->setDisabled(true);
	if (file_exist(_settings.getValueOfParam<std::string>(CAPTUREPARAM::CAP_VIDEO_FILE))){
		initCapture();
	}
}

void BioTracker::initGui()
{
	setPlayfieldPaused(true);
	ui.actionOpen_Video->setEnabled(true);
	ui.frame_num_edit->setEnabled(false);
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
	QObject::connect(ui.frame_num_edit, SIGNAL( returnPressed() ), this, SLOT( changeCurrentFramebyEdit()));

	//slider
	QObject::connect(ui.sld_video, SIGNAL(sliderPressed()),this, SLOT(pauseCapture()));
	QObject::connect(ui.sld_video, SIGNAL( sliderReleased() ), this, SLOT( changeCurrentFramebySlider()));
	QObject::connect(ui.sld_speed, SIGNAL( sliderMoved(int) ), this, SLOT( changeFps(int)));


	//tracking thread signals
	QObject::connect(_trackingThread, SIGNAL(notifyGUI(std::string, MSGS::MTYPE)), this, SLOT(printGuiMessage(std::string, MSGS::MTYPE)));
	QObject::connect(this, SIGNAL(videoPause(bool)), _trackingThread, SLOT(enableVideoPause(bool)));
	QObject::connect(this, SIGNAL(videoStop()), _trackingThread, SLOT(stopCapture()));
	QObject::connect(_trackingThread, SIGNAL( trackingSequenceDone(cv::Mat) ), this, SLOT( drawImage(cv::Mat) ));
	QObject::connect(_trackingThread, SIGNAL( newFrameNumber(int) ), this, SLOT( updateFrameNumber(int) ));
	QObject::connect(_trackingThread, SIGNAL( sendFps(int) ), this, SLOT( showFps(int) ));
	QObject::connect(this, SIGNAL( nextFrameReady(bool) ), _trackingThread, SLOT( enableHandlingNextFrame(bool) ));
	QObject::connect(this, SIGNAL( changeFrame(int) ), _trackingThread, SLOT( nextFrame() ));
	QObject::connect(this, SIGNAL( grabNextFrame()), _trackingThread, SLOT( nextFrame() ));
	QObject::connect(this, SIGNAL( fpsChange(int)), _trackingThread, SLOT( setFps(int) ));
	
}

void BioTracker::browseVideo()
{
	stopCapture();
	QString filename = QFileDialog::getOpenFileName(this, tr("Open video"), "", tr("video Files (*.avi *.wmv)"));
	if(filename.compare("") != 0){
		_settings.setParam(CAPTUREPARAM::CAP_VIDEO_FILE,filename.toStdString());
		initCapture();
	}
	
}

void BioTracker::setPlayfieldPaused(bool enabled){
	switch(enabled)
	{
		//video is paused
		case true:			
			ui.button_nextFrame->setEnabled(true);
			ui.button_previousFrame->setEnabled(true);
			ui.button_playPause->setIcon(_iconPlay);
			break;
		//video is playing
		case false:			
			ui.button_nextFrame->setEnabled(false);
			ui.button_previousFrame->setEnabled(false);
			ui.button_playPause->setIcon(_iconPause);
			break;
	}
}

void BioTracker::runCapture()
{	
	//check if video is stopped
	if (_videoStopped){
		initCapture();
		_videoPaused=false;
		setPlayfieldPaused(false);
		emit videoPause(false);
	}
	
	//if not stopped resume/pause video
	else
	{	
		_videoPaused = !_videoPaused;
		if (_videoPaused){				
			setPlayfieldPaused(true);
		}
		else
		{			
			setPlayfieldPaused(false);
		}
		emit videoPause(_videoPaused);
	}
	
}

void BioTracker::initCapture()
{
	_currentFrame = _settings.getValueOfParam<int>(CAPTUREPARAM::CAP_PAUSED_AT_FRAME);
	_videoStopped = false;
	_videoPaused = true;
	emit videoPause(true);
	_trackingThread->startCapture();
	ui.sld_video->setMaximum(_trackingThread->getVideoLength());		
	ui.sld_video->setDisabled(false);
	updateFrameNumber(_currentFrame);
	emit changeFrame(_currentFrame);
	ui.frame_num_edit->setValidator( new QIntValidator(0, _trackingThread->getVideoLength(), this) );
	ui.frame_num_edit->setEnabled(true);
	ui.sld_speed->setValue(_trackingThread->getFps());
	ui.fps_label->setText(StringHelper::toQString(StringHelper::iToSS(_trackingThread->getFps())));
	setPlayfieldPaused(true);
}

void BioTracker::stepCaptureForward()
{
	//if video not yet loaded, load it now!
	if(_videoStopped){
		initCapture();
	}
	_videoPaused = true;
	emit grabNextFrame();
	_settings.setParam(CAPTUREPARAM::CAP_PAUSED_AT_FRAME,StringHelper::iToSS(_currentFrame));
}

void BioTracker::stepCaptureBackward()
{
	if(_currentFrame > 0)
	{
		updateFrameNumber(_currentFrame-1);
		emit changeFrame(_currentFrame);		
		_settings.setParam(CAPTUREPARAM::CAP_PAUSED_AT_FRAME,StringHelper::iToSS(_currentFrame));
	}
}

void BioTracker::pauseCapture()
{
	emit videoPause(true);
	_videoPaused = true;
	setPlayfieldPaused(true);
	_settings.setParam(CAPTUREPARAM::CAP_PAUSED_AT_FRAME,StringHelper::iToSS(_currentFrame));
}


void BioTracker::stopCapture()
{	
	_videoStopped = true;
	_videoPaused = true;	
	emit videoStop();
	if(!_trackingThread->wait(5000)) //Wait until it actually has terminated (max. 5 sec)
	{
		printGuiMessage("Thread deadlock detected! Terminating now!",MSGS::FAIL);
		_trackingThread->terminate(); //Thread didn't exit in time, probably deadlocked, terminate it!
		_trackingThread->wait(); //Note: We have to wait again here!
	}
	setPlayfieldPaused(true);
	updateFrameNumber(0);
	ui.sld_video->setDisabled(true);
	_settings.setParam(CAPTUREPARAM::CAP_PAUSED_AT_FRAME,StringHelper::iToSS(_currentFrame));
}

void BioTracker::updateFrameNumber(int frameNumber)
{
	_currentFrame = frameNumber;
	ui.sld_video->setValue(_currentFrame);
	ui.frame_num_edit->setText(StringHelper::toQString(StringHelper::iToSS(_currentFrame)));
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

void BioTracker::changeCurrentFramebySlider()
{	
	int value = ui.sld_video->value();
	if(_trackingThread->isReadyForNextFrame())
		emit changeFrame(value);
	_settings.setParam(CAPTUREPARAM::CAP_PAUSED_AT_FRAME,StringHelper::iToSS(_currentFrame));
	updateFrameNumber(value);
}
void BioTracker::changeCurrentFramebyEdit()
{	
	QString valueStr = ui.frame_num_edit->text();
	if(StringHelper::isNumber(valueStr))
	{
		int value = valueStr.toInt();
		if(_trackingThread->isReadyForNextFrame())
			emit changeFrame(value);
		_settings.setParam(CAPTUREPARAM::CAP_PAUSED_AT_FRAME,StringHelper::iToSS(_currentFrame));
		updateFrameNumber(value);
	}	
}

void BioTracker::showFps(int fps)
{
	//show actual fps
	ui.fps_edit->setText(StringHelper::toQString(StringHelper::iToSS(fps)));
}
void BioTracker::changeFps(int fps)
{
	//show target fps
	ui.fps_label->setText(StringHelper::toQString(StringHelper::iToSS(fps)));
	emit fpsChange(fps);
}