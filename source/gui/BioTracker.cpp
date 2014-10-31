#include "source/gui/BioTracker.h"
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>

#include "source/tracking/algorithm/algorithms.h"

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>

BioTracker::BioTracker(Settings &settings,QWidget *parent, Qt::WindowFlags flags) : 
	QMainWindow(parent, flags),
	_trackingThread(nullptr),
	_settings(settings)
{
	ui.setupUi(this);
	setPlayfieldEnabled(false);
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
	_isPanZoomMode = false;
	_trackingThread = new TrackingThread(_settings);
	_iconPause.addFile(QStringLiteral(":/BioTracker/resources/pause-sign.png"), QSize(), QIcon::Normal, QIcon::Off);
	_iconPlay.addFile(QStringLiteral(":/BioTracker/resources/arrow-forward1.png"), QSize(), QIcon::Normal, QIcon::Off);
	_vboxParams = new QVBoxLayout();
	ui.groupBox_params->setLayout(_vboxParams);	
	_vboxTools = new QVBoxLayout();
	ui.groupBox_tools->setLayout(_vboxTools);
	//meta types
	qRegisterMetaType<cv::Mat>("cv::Mat");
	qRegisterMetaType<MSGS::MTYPE>("MSGS::MTYPE");
	qRegisterMetaType<std::string>("std::string");
	initGui();
	initConnects();
	ui.sld_video->setDisabled(true);
	if (file_exist(_settings.getValueOfParam<std::string>(CAPTUREPARAM::CAP_VIDEO_FILE))){
		setPlayfieldEnabled(true);		
		initCapture();
	}
}

void BioTracker::initGui()
{
	initAlgorithms();
	//setPlayfieldPaused(true);
	ui.actionOpen_Video->setEnabled(true);
	ui.frame_num_edit->setEnabled(false);
}

void BioTracker::initConnects()
{
	//File -> Open Video
	QObject::connect(ui.actionOpen_Video, SIGNAL(triggered()), this, SLOT(browseVideo()));
	QObject::connect(ui.actionOpen_Picture, SIGNAL(triggered()), this, SLOT(browsePicture()));	
	//video playfield buttons
	QObject::connect(ui.button_playPause, SIGNAL(clicked()), this, SLOT(runCapture()));
	QObject::connect(ui.button_stop, SIGNAL(clicked()), this, SLOT(stopCapture()));
	QObject::connect(ui.button_nextFrame, SIGNAL( clicked() ), this, SLOT(stepCaptureForward())); 
	QObject::connect(ui.button_previousFrame, SIGNAL( clicked() ), this, SLOT(stepCaptureBackward()));
	QObject::connect(ui.frame_num_edit, SIGNAL( returnPressed() ), this, SLOT( changeCurrentFramebyEdit()));
	QObject::connect(ui.button_screenshot, SIGNAL( clicked() ), this, SLOT( takeScreenshot()));
	QObject::connect(ui.cb_algorithms, SIGNAL( currentIndexChanged ( QString) ), this, SLOT(trackingAlgChanged(QString)));
	QObject::connect(ui.button_panZoom, SIGNAL(  clicked() ), this, SLOT(switchPanZoomMode()));

	//slider
	QObject::connect(ui.sld_video, SIGNAL(sliderPressed()),this, SLOT(pauseCapture()));
	QObject::connect(ui.sld_video, SIGNAL( sliderMoved(int) ), this, SLOT( updateFrameNumber(int)));
	QObject::connect(ui.sld_video, SIGNAL( sliderReleased() ), this, SLOT( changeCurrentFramebySlider()));
	QObject::connect(ui.sld_video, SIGNAL( actionTriggered(int) ), this, SLOT( changeCurrentFramebySlider(int)));	
	QObject::connect(ui.sld_speed, SIGNAL( valueChanged(int) ), this, SLOT( changeFps(int)));
	QObject::connect(ui.videoView, SIGNAL(notifyGUI(std::string, MSGS::MTYPE)), this, SLOT(printGuiMessage(std::string, MSGS::MTYPE)));

	//tracking thread signals
	QObject::connect(_trackingThread, SIGNAL(notifyGUI(std::string, MSGS::MTYPE)), this, SLOT(printGuiMessage(std::string, MSGS::MTYPE)));
	QObject::connect(this, SIGNAL(videoPause(bool)), _trackingThread, SLOT(enableVideoPause(bool)));
	QObject::connect(this, SIGNAL(videoStop()), _trackingThread, SLOT(stopCapture()));
	QObject::connect(_trackingThread, SIGNAL( trackingSequenceDone(cv::Mat) ), this, SLOT( drawImage(cv::Mat) ));
	QObject::connect(_trackingThread, SIGNAL( newFrameNumber(int) ), this, SLOT( updateFrameNumber(int) ));
	QObject::connect(_trackingThread, SIGNAL( sendFps(double) ), this, SLOT( showFps(double) ));
	QObject::connect(this, SIGNAL( nextFrameReady(bool) ), _trackingThread, SLOT( enableHandlingNextFrame(bool) ));
	QObject::connect(this, SIGNAL( changeFrame(int) ), _trackingThread, SLOT( setFrameNumber(int) ));
	QObject::connect(this, SIGNAL( grabNextFrame()), _trackingThread, SLOT( nextFrame() ));
	QObject::connect(this, SIGNAL( fpsChange(double)), _trackingThread, SLOT( setFps(double) ));
	QObject::connect(this, SIGNAL ( enableMaxSpeed(bool)), _trackingThread, SLOT(setMaxSpeed(bool) ));
	QObject::connect(this, SIGNAL ( changeTrackingAlg(std::shared_ptr<TrackingAlgorithm>) ), _trackingThread, SLOT(setTrackingAlgorithm(std::shared_ptr<TrackingAlgorithm>) ));
	QObject::connect(this, SIGNAL ( changeTrackingAlg(std::shared_ptr<TrackingAlgorithm>) ), ui.videoView, SLOT(setTrackingAlgorithm(std::shared_ptr<TrackingAlgorithm>) ));
	QObject::connect(_trackingThread, SIGNAL ( invalidFile() ), this, SLOT( invalidFile() ));	

}

void BioTracker::initAlgorithms()
{
    // add NoTracker first
    for (auto& algByStr : Algorithms::byString)
    {
        if (algByStr.second == Algorithms::NoTracking)
        {
            ui.cb_algorithms->addItem(algByStr.first);
            break;
        }
    }

    // add Trackers
    for (auto& algByStr : Algorithms::byString)
    {
        if (algByStr.second != Algorithms::NoTracking)
        {
            ui.cb_algorithms->addItem(algByStr.first);
        }
    }
}

void BioTracker::browseVideo()
{
	stopCapture();
	QString filename = QFileDialog::getOpenFileName(this, tr("Open video"), "", tr("video Files (*.avi *.wmv *.mp4)"));
	if(filename.compare("") != 0){
		_settings.setParam(CAPTUREPARAM::CAP_VIDEO_FILE,filename.toStdString());
		_settings.setParam(GUIPARAM::IS_SOURCE_VIDEO,"true");
		setPlayfieldEnabled(true);
		initCapture();
	}	
}
void BioTracker::browsePicture()
{
	stopCapture();
	QStringList  filenames = QFileDialog::getOpenFileNames(this, tr("Open video"), "", 
		tr("image Files (*.png *.jpg *.jpeg *.gif *.bmp *.jpe *.ppm *.tiff *.tif *.sr *.ras *.pbm *.pgm *.jp2 *.dib)"));
	if(!filenames.isEmpty()){
		_settings.setParam(PICTUREPARAM::PICTURE_FILE,filenames.first().toStdString());
		_settings.setParam(GUIPARAM::IS_SOURCE_VIDEO,"false");
		setPlayfieldEnabled(true);
		initPicture(filenames);
	}	
}
void BioTracker::initPicture(QStringList filenames)
{	
	_videoStopped = false;
	_videoPaused = true;
	emit videoPause(true);
	_trackingThread->loadPictures(filenames);
	ui.sld_video->setMaximum(_trackingThread->getVideoLength()-1);		
	ui.sld_video->setDisabled(false);
	ui.sld_video->setPageStep(static_cast<int>(_trackingThread->getVideoLength()/20));
	updateFrameNumber(_currentFrame);
	emit changeFrame(_currentFrame);
	ui.frame_num_edit->setValidator( new QIntValidator(0, _trackingThread->getVideoLength()-1, this) );
	ui.frame_num_edit->setEnabled(true);
	ui.sld_speed->setValue(_trackingThread->getFps());
	std::stringstream ss;
	double fps = _trackingThread->getFps();
	ss << std::setprecision(5) << fps;
	ui.fps_label->setText(QString::fromStdString(ss.str()));
	setPlayfieldPaused(true);
	ui.videoView->fitToWindow();
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
	ui.sld_video->setMaximum(_trackingThread->getVideoLength()-1);		
	ui.sld_video->setDisabled(false);
	ui.sld_video->setPageStep(static_cast<int>(_trackingThread->getVideoLength()/20));
	updateFrameNumber(_currentFrame);
	emit changeFrame(_currentFrame);
	ui.frame_num_edit->setValidator( new QIntValidator(0, _trackingThread->getVideoLength()-1, this) );
	ui.frame_num_edit->setEnabled(true);
	ui.sld_speed->setValue(_trackingThread->getFps());
	std::stringstream ss;
	double fps = _trackingThread->getFps();
	ss << std::setprecision(5) << fps;
	ui.fps_label->setText(QString::fromStdString(ss.str()));
	setPlayfieldPaused(true);
	ui.videoView->fitToWindow();
}
void BioTracker::invalidFile()
{
	setPlayfieldEnabled(false);
	_videoStopped = true;
}
void BioTracker::setPlayfieldEnabled(bool enabled)
{
	ui.button_playPause->setEnabled(enabled);
	ui.button_nextFrame->setEnabled(enabled);
	ui.button_screenshot->setEnabled(enabled);
	ui.button_previousFrame->setEnabled(enabled);
	ui.button_stop->setEnabled(enabled);
}
void BioTracker::stepCaptureForward()
{
	//if video not yet loaded, load it now!
	if(_videoStopped){
		initCapture();
	}
	_videoPaused = true;
	emit grabNextFrame();
	_settings.setParam(CAPTUREPARAM::CAP_PAUSED_AT_FRAME,QString::number(_currentFrame).toStdString());
}

void BioTracker::stepCaptureBackward()
{
	if(_currentFrame > 0)
	{
		updateFrameNumber(_currentFrame-1);
		emit changeFrame(_currentFrame);		
		_settings.setParam(CAPTUREPARAM::CAP_PAUSED_AT_FRAME,QString::number(_currentFrame).toStdString());
	}
}

void BioTracker::pauseCapture()
{
	emit videoPause(true);
	_videoPaused = true;
	setPlayfieldPaused(true);
	_settings.setParam(CAPTUREPARAM::CAP_PAUSED_AT_FRAME,QString::number(_currentFrame).toStdString());
}


void BioTracker::stopCapture()
{	
	_videoStopped = true;
	_videoPaused = true;	
	emit videoStop();
	setPlayfieldPaused(true);
	updateFrameNumber(0);
	ui.sld_video->setDisabled(true);
	_settings.setParam(CAPTUREPARAM::CAP_PAUSED_AT_FRAME,QString::number(_currentFrame).toStdString());
	ui.cb_algorithms->setCurrentIndex(0);
    trackingAlgChanged(Algorithms::NoTracking);
}

void BioTracker::updateFrameNumber(int frameNumber)
{
	_currentFrame = frameNumber;
	ui.sld_video->setValue(_currentFrame);
	ui.frame_num_edit->setText(QString::number(_currentFrame));
	if(frameNumber == ui.sld_video->maximum())
	{

		emit videoPause(true);
		_videoPaused = true;
		setPlayfieldPaused(true);
		_settings.setParam(CAPTUREPARAM::CAP_PAUSED_AT_FRAME,QString::number(0).toStdString());

	}
}

void BioTracker::drawImage(cv::Mat image)
{


	if(image.data)
	{	
		ui.videoView->showImage(image);

	}

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
	msgLine.append(QString::fromStdString(message));
	ui.edit_notification->append(msgLine);
}

void BioTracker::changeCurrentFramebySlider()
{	
	int value = ui.sld_video->value();	
	emit changeFrame(value);
	updateFrameNumber(value);
	_settings.setParam(CAPTUREPARAM::CAP_PAUSED_AT_FRAME,QString::number(_currentFrame).toStdString());

}
void BioTracker::changeCurrentFramebySlider(int SliderAction)
{
	int fNum = _currentFrame;
	switch(SliderAction)
	{
		//SliderPageStepAdd
	case 3:
		fNum += ui.sld_video->pageStep();
		if (fNum > ui.sld_video->maximum())
			fNum = ui.sld_video->maximum();
		changeCurrentFramebySlider();
		break;
		//SliderPageStepSub
	case 4:
		fNum -= ui.sld_video->pageStep();
		if(fNum < 0 )
			fNum = 0;
		changeCurrentFramebySlider();
		break;
	default:
		break;
	}
	updateFrameNumber(fNum);	
}
void BioTracker::changeCurrentFramebyEdit()
{	
	QString valueStr = ui.frame_num_edit->text();
	//check if string is a number by using regular expression
	QRegExp re("\\d*");  // a digit (\d), zero or more times (*)
	if(re.exactMatch(valueStr))
	{
		int value = valueStr.toInt();
		if(_trackingThread->isReadyForNextFrame())
			emit changeFrame(value);
		_settings.setParam(CAPTUREPARAM::CAP_PAUSED_AT_FRAME,QString::number(_currentFrame).toStdString());
		updateFrameNumber(value);
	}	
}

void BioTracker::showFps(double fps)
{
	std::stringstream ss;
	ss << std::setprecision(5) << fps;
	//show actual fps
	ui.fps_edit->setText(QString::fromStdString(ss.str()));
}
void BioTracker::changeFps(int fps)
{
	//maximum slider position will enable maxSpeed
	if(fps > 60)
	{
		emit enableMaxSpeed(true);
		ui.fps_label->setText("max");
	}
	else
	{
		//show target fps
		ui.fps_label->setText(QString::number(fps));
		emit enableMaxSpeed(false);
		emit fpsChange((double)fps);
	}
}

void BioTracker::trackingAlgChanged(QString trackingAlgStr)
{
    trackingAlgChanged(Algorithms::byString.at(trackingAlgStr));
}

void BioTracker::trackingAlgChanged(Algorithms::Type trackingAlg)
{
	//first remove ui containers of old algorithm
	if(_tracker)
	{
		_vboxParams->removeWidget(_paramsWidget);
		_vboxTools->removeWidget(_toolsWidget);
		delete _paramsWidget;
		delete _toolsWidget;
	}

    if (trackingAlg == Algorithms::NoTracking)
    {
        _tracker.reset();
    } else
    {
        // restore previous state
        std::string path;
        std::vector<TrackedObject> storedObjects;
        if (_serializationTmpFileMap.count(trackingAlg))
        {
            auto& file = _serializationTmpFileMap.at(trackingAlg);
            assert(file.open());
            path = file.fileName().toStdString();
            std::cout << "Trying to restore from: " << path << std::endl;
            {
                std::ifstream is(path);
                cereal::XMLInputArchive ar(is);
                ar(storedObjects);
            }
        } else
        {
            // create a new QTemporaryFile and return a reference
            QTemporaryFile& tmpFile = _serializationTmpFileMap[trackingAlg];
            if (tmpFile.open()) path = tmpFile.fileName().toStdString();
            else assert(false);
        }
        _tracker = std::shared_ptr<TrackingAlgorithm>(
            (Algorithms::byType.at(trackingAlg))(_settings, path, this));
        _tracker->loadObjects(std::move(storedObjects));

        connectTrackingAlg(_tracker);
    }

    ui.groupBox_params->repaint();
    ui.groupBox_tools->repaint();
	emit changeTrackingAlg(_tracker);
}

void BioTracker::connectTrackingAlg(std::shared_ptr<TrackingAlgorithm> tracker)
{	
	QObject::connect(ui.videoView,		SIGNAL ( pressEvent			(QMouseEvent*) ), 
        tracker.get(), SLOT(mousePressEvent		(QMouseEvent*) ));
	QObject::connect(ui.videoView,		SIGNAL ( releaseEvent		(QMouseEvent*) ), 
        tracker.get(), SLOT(mouseReleaseEvent	(QMouseEvent*) ));
	QObject::connect(ui.videoView,		SIGNAL ( moveEvent			(QMouseEvent*) ), 
        tracker.get(), SLOT(mouseMoveEvent		(QMouseEvent*) ));
	QObject::connect(ui.videoView,		SIGNAL ( mouseWheelEvent	(QWheelEvent*) ), 
        tracker.get(), SLOT(mouseWheelEvent		(QWheelEvent*) ));

    QObject::connect(tracker.get(), SIGNAL(notifyGUI(std::string, MSGS::MTYPE)),
		this, SLOT(printGuiMessage(std::string, MSGS::MTYPE)));
    QObject::connect( tracker.get(), SIGNAL( update() ),
		ui.videoView, SLOT( updateGL() ));
    QObject::connect(tracker.get(),		SIGNAL ( requestCurrentScreen() ),
		ui.videoView, SLOT( getCurrentScreen() ));
	
	if(_tracker)
	{
		try
		{
			_paramsWidget = _tracker->getParamsWidget();
			_vboxParams->addWidget(_paramsWidget);
			_toolsWidget = _tracker->getToolsWidget();
			_vboxTools->addWidget(_toolsWidget);
		}
		catch(std::exception&)
		{
			emit printGuiMessage("cannot create UI elements for selected algorithm",MSGS::FAIL);
		}
	}
}

void BioTracker::takeScreenshot()
{
	const std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
	const std::time_t t = std::chrono::system_clock::to_time_t(p);
	std::string dateTime = std::ctime(&t);
	// ctime adds a newline to the string due to historical reasons
	dateTime = dateTime.substr(0, dateTime.size() - 1);
	QString filepath = _settings.getValueOfParam<QString>(CAPTUREPARAM::CAP_SCREENSHOT_PATH);
	filepath.append("/screenshot_").append(QString::fromStdString(dateTime)).append(".png");
	ui.videoView->takeScreenshot(filepath);
}

void BioTracker::switchPanZoomMode()
{
	_isPanZoomMode = !_isPanZoomMode;
	ui.videoView->setPanZoomMode(_isPanZoomMode);
}
