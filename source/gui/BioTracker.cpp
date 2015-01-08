#include "source/gui/BioTracker.h"

#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>

#include <QCryptographicHash>
#include <QFileInfo>

#include "source/settings/Settings.h"
#include "source/utility/stdext.h"
#include "source/tracking/TrackingThread.h"
#include "source/tracking/algorithm/algorithms.h"
#include "source/tracking/serialization/SerializationData.h"

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

using GUIPARAM::MediaType;

BioTracker::BioTracker(Settings &settings, QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
    , _settings(settings)
    , _trackingThread(std::make_unique<TrackingThread>(_settings))
    , _videoMode(VideoMode::Stopped)
    , _mediaType(_settings.getValueOrDefault<MediaType>(GUIPARAM::MEDIA_TYPE, MediaType::NoMedia))
    , _isPanZoomMode(false)
    , _currentFrame(0)
{
	ui.setupUi(this);

	initGui();
	initConnects();

	if (_mediaType != MediaType::NoMedia) initPlayback();

	{
		QFile file(QString::fromStdString(CONFIGPARAM::GEOMETRY_FILE));
		if (file.open(QIODevice::ReadOnly)) restoreGeometry(file.readAll());
	}
	{
		QFile file(QString::fromStdString(CONFIGPARAM::STATE_FILE));
		if (file.open(QIODevice::ReadOnly)) restoreState(file.readAll());
	}
}

BioTracker::~BioTracker()
{
	_trackingThread->stop();
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

void BioTracker::initGui()
{
	_iconPause.addFile(QStringLiteral(":/BioTracker/resources/pause-sign.png"), QSize(), QIcon::Normal, QIcon::Off);
	_iconPlay.addFile(QStringLiteral(":/BioTracker/resources/arrow-forward1.png"), QSize(), QIcon::Normal, QIcon::Off);

	_vboxParams = new QVBoxLayout(ui.groupBox_params);
	_vboxTools  = new QVBoxLayout(ui.groupBox_tools);

	initAlgorithmList();
}

void BioTracker::initConnects()
{
	//File -> Open Video
	QObject::connect(ui.actionOpen_Video, SIGNAL(triggered()), this, SLOT(browseVideo()));
	QObject::connect(ui.actionOpen_Picture, SIGNAL(triggered()), this, SLOT(browsePicture()));	
	//File -> Load/Store tracking data
	QObject::connect(ui.actionLoad_tracking_data, &QAction::triggered,
	                 this, &BioTracker::loadTrackingDataTriggered);
	QObject::connect(ui.actionSave_tracking_data, &QAction::triggered,
	                 this, &BioTracker::storeTrackingDataTriggered);
	//File -> Exit
	QObject::connect(ui.actionQuit, &QAction::triggered, this, &BioTracker::exit);
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
	QObject::connect(ui.sld_video, SIGNAL(sliderMoved(int) ), this, SLOT(updateFrameNumber(int)));
	QObject::connect(ui.sld_video, SIGNAL(sliderReleased() ), this, SLOT(changeCurrentFramebySlider()));
	QObject::connect(ui.sld_video, SIGNAL(actionTriggered(int) ), this, SLOT(changeCurrentFramebySlider(int)));
	QObject::connect(ui.sld_speed, SIGNAL(valueChanged(int) ), this, SLOT(changeFps(int)));
	QObject::connect(ui.videoView, SIGNAL(notifyGUI(std::string, MSGS::MTYPE)), this, SLOT(printGuiMessage(std::string, MSGS::MTYPE)));

	//tracking thread signals
	QObject::connect(_trackingThread.get(), &TrackingThread::notifyGUI, this, &BioTracker::printGuiMessage);
	QObject::connect(this, &BioTracker::videoPause, _trackingThread.get(), &TrackingThread::enableVideoPause);
	QObject::connect(this, &BioTracker::videoStop, _trackingThread.get(), &TrackingThread::stopCapture);
	QObject::connect(_trackingThread.get(), &TrackingThread::trackingSequenceDone, this, &BioTracker::drawImage);
	QObject::connect(_trackingThread.get(), &TrackingThread::newFrameNumber, this, &BioTracker::updateFrameNumber);
	QObject::connect(_trackingThread.get(), &TrackingThread::sendFps, this, &BioTracker::showFps);
	QObject::connect(this, &BioTracker::nextFrameReady, _trackingThread.get(), &TrackingThread::enableHandlingNextFrame);
	QObject::connect(this, &BioTracker::changeFrame, _trackingThread.get(), &TrackingThread::setFrameNumber);
	QObject::connect(this, &BioTracker::grabNextFrame, _trackingThread.get(), &TrackingThread::nextFrame);
	QObject::connect(this, &BioTracker::fpsChange, _trackingThread.get(), &TrackingThread::setFps);
	QObject::connect(this, &BioTracker::enableMaxSpeed, _trackingThread.get(), &TrackingThread::setMaxSpeed);
	QObject::connect(this, &BioTracker::changeTrackingAlg, _trackingThread.get(), &TrackingThread::setTrackingAlgorithm);
	QObject::connect(this, &BioTracker::changeTrackingAlg, ui.videoView, &VideoView::setTrackingAlgorithm);
	QObject::connect(_trackingThread.get(), &TrackingThread::invalidFile, this, &BioTracker::invalidFile);
	QObject::connect(_trackingThread.get(), &TrackingThread::fileNameChange, this, &BioTracker::displayFileName);

	/*	 _______________________
	*	|						|
	*	| connect shortcut keys	|
	*	|_______________________| */
	// Pan&Zoom
	QShortcut *shortcutPan = new QShortcut(QKeySequence
		(QString::fromStdString(_settings.getValueOrDefault<std::string>(GUIPARAM::SHORTCUT_ZOOM,"Z"))), this);
	QObject::connect(shortcutPan, SIGNAL(activated()), ui.button_panZoom, SLOT(click()));
	// Play*Pause
	QShortcut *shortcutPlay = new QShortcut(QKeySequence
		(QString::fromStdString(_settings.getValueOrDefault<std::string>(GUIPARAM::SHORTCUT_PLAY,"Space"))), this);
	QObject::connect(shortcutPlay, SIGNAL(activated()), ui.button_playPause, SLOT(click()));
	// Next Frame
	QShortcut *shortcutNext = new QShortcut(QKeySequence
		(QString::fromStdString(_settings.getValueOrDefault<std::string>(GUIPARAM::SHORTCUT_NEXT,"Right"))), this);
	QObject::connect(shortcutNext, SIGNAL(activated()), ui.button_nextFrame, SLOT(click()));
	// Previous Frame
	QShortcut *shortcutPrev = new QShortcut(QKeySequence
		(QString::fromStdString(_settings.getValueOrDefault<std::string>(GUIPARAM::SHORTCUT_PREV,"Left"))), this);
	QObject::connect(shortcutPrev, SIGNAL(activated()), ui.button_previousFrame, SLOT(click()));
}

void BioTracker::initPlayback()
{
	_currentFrame = _settings.getValueOrDefault<size_t>(GUIPARAM::PAUSED_AT_FRAME, 0);
	updateFrameNumber(static_cast<int>(_currentFrame));
	_trackingThread->enableVideoPause(true);

	switch (_mediaType) {
	case MediaType::Images:
		_trackingThread->loadPictures(_settings.getVectorOfParam<std::string>(PICTUREPARAM::PICTURE_FILES));
		break;
	case MediaType::Video:
		_trackingThread->loadVideo(_settings.getValueOfParam<std::string>(CAPTUREPARAM::CAP_VIDEO_FILE));
		break;
	default:
		assert(false);
		break;
	}

	setPlayfieldEnabled(true);
	ui.sld_video->setMaximum(_trackingThread->getVideoLength()-1);
	ui.sld_video->setDisabled(false);
	ui.sld_video->setPageStep(static_cast<int>(_trackingThread->getVideoLength()/20));

	ui.frame_num_edit->setValidator(new QIntValidator(0, _trackingThread->getVideoLength()-1, this));
	ui.frame_num_edit->setEnabled(true);
	ui.sld_speed->setValue(static_cast<int>(_trackingThread->getFps()));

	std::stringstream ss;
	double fps = _trackingThread->getFps();
	ss << std::setprecision(5) << fps;
	ui.fps_label->setText(QString::fromStdString(ss.str()));

	setPlayfieldPaused(true);

	_trackingThread->setFrameNumber(static_cast<int>(_currentFrame));

	ui.videoView->fitToWindow();
}

void BioTracker::initAlgorithmList()
{
	// add NoTracker first
	ui.cb_algorithms->addItem(QString::fromStdString(
	    Algorithms::Registry::getInstance().stringByType().at(Algorithms::NoTracking)));

	// add Trackers
	for (auto& algByStr : Algorithms::Registry::getInstance().typeByString())
	{
		if (algByStr.second != Algorithms::NoTracking)
		{
			ui.cb_algorithms->addItem(QString::fromStdString(algByStr.first));
		}
	}
}

void BioTracker::browseVideo()
{
	stopCapture();
	QString filename = QFileDialog::getOpenFileName(this, tr("Open video"), "", tr("video Files (*.avi *.wmv *.mp4)"));
	if(!filename.isEmpty()) {
		_mediaType = MediaType::Video;
		_settings.setParam(CAPTUREPARAM::CAP_VIDEO_FILE, filename.toStdString());
		_settings.setParam(GUIPARAM::MEDIA_TYPE, MediaType::Video);
		initPlayback();
	}
}

void BioTracker::browsePicture()
{
	stopCapture();
	QStringList  filenames = QFileDialog::getOpenFileNames(this, tr("Open video"), "",
		tr("image Files (*.png *.jpg *.jpeg *.gif *.bmp *.jpe *.ppm *.tiff *.tif *.sr *.ras *.pbm *.pgm *.jp2 *.dib)"));
	if(!filenames.isEmpty()){
		std::vector<std::string> filenamevec;
		for (QString& file : filenames) {
			filenamevec.push_back(file.toStdString());
		}
		_mediaType = MediaType::Images;
		_settings.setParamVector<std::string>(PICTUREPARAM::PICTURE_FILES, std::move(filenamevec));
		_settings.setParam(GUIPARAM::MEDIA_TYPE, MediaType::Images);
		initPlayback();
	}
}

void BioTracker::loadTrackingDataTriggered(bool /* checked */)
{
	if (!_tracker) {
		QMessageBox::warning(this, "Unable to load tracking data",
		                     "No tracker selected.");
		return;
	}

	QString filename = QFileDialog::getOpenFileName(this, tr("Load tracking data"), "", tr("Data Files (*.tdat)"));
	if (!filename.isEmpty()) {
		loadTrackingData(filename.toStdString());
	}
}

void BioTracker::loadTrackingData(const std::string &filename)
{
	assert(_tracker->getType());

	printGuiMessage("Restoring tracking data from " + filename, MSGS::NOTIFICATION);
	std::ifstream is(filename);
	cereal::JSONInputArchive ar(is);

	Serialization::Data sdata;
	ar(sdata);

	const std::string trackerType =
	        Algorithms::Registry::getInstance().stringByType().at(_tracker->getType().get());

	if (sdata.getType() != trackerType) {
		QMessageBox::warning(this, "Unable to load tracking data",
		                     "Tracker type does not match.");
		return;
	}

	const boost::optional<std::vector<std::string>> currentFiles = getOpenFiles();

	if (!currentFiles) {
		QMessageBox::warning(this, "Unable to load tracking data",
		                     "No file opened.");
		return;
	}

	const boost::optional<std::string> hash = getFileHash(currentFiles.get().front(), currentFiles.get().size());

	if (!hash) {
		QMessageBox::warning(this, "Unable to load tracking data",
							 "Could not calculate file hash.");
		return;
	}

	if (sdata.getFileSha1Hash() != hash.get()) {
		auto buttonClicked = QMessageBox::warning(this, "Unable to load tracking data",
												  "File hash does not match", QMessageBox::Ok | QMessageBox::Ignore);
		if (buttonClicked == QMessageBox::Ok)
			return;
	}

	_tracker->loadObjects(sdata.getTrackedObjects());
}

void BioTracker::storeTrackingDataTriggered(bool /* checked */)
{
	if (!_tracker) {
		QMessageBox::warning(this, "Unable to store tracking data",
		                     "No tracker selected.");
		return;
	}

	QFileDialog dialog(this, tr("Save tracking data"));
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setDefaultSuffix("tdat");
	dialog.setNameFilter(tr("Data Files (*.tdat)"));
	if (dialog.exec()) {
		const QStringList filenames = dialog.selectedFiles();
		if (!filenames.empty()) {
			storeTrackingData(filenames.first().toStdString());
		}
	}
}

void BioTracker::storeTrackingData(const std::string &filename)
{
	assert(_tracker->getType());
	printGuiMessage("Storing tracking data in " + filename, MSGS::NOTIFICATION);

	const std::string trackerType =
	        Algorithms::Registry::getInstance().stringByType().at(_tracker->getType().get());
	const boost::optional<std::vector<std::string>> currentFiles = getOpenFiles();

	if (!currentFiles) {
		QMessageBox::warning(this, "Unable to store tracking data",
		                     "No file opened.");
		return;
	}

	const boost::optional<std::string> hash = getFileHash(currentFiles.get().front(), currentFiles.get().size());

	if (!hash) {
		QMessageBox::warning(this, "Unable to store tracking data",
		                     "Could not calculate file hash.");
		return;
	}

	Serialization::Data sdata(trackerType, hash.get(), getFilenamesFromPaths(currentFiles.get()), _tracker->getObjects());

	std::ofstream ostream(filename, std::ios::binary);
	cereal::JSONOutputArchive archive(ostream);
	archive(std::move(sdata));
}

boost::optional<std::vector<std::string>> BioTracker::getOpenFiles() const
{
	boost::optional<std::vector<std::string>> filename;
	switch (_mediaType) {
	case MediaType::Images:
		filename = _settings.getVectorOfParam<std::string>(PICTUREPARAM::PICTURE_FILES);
		break;
	case MediaType::Video:
		filename = std::vector<std::string>();
		filename.get().push_back(_settings.getValueOfParam<std::string>(CAPTUREPARAM::CAP_VIDEO_FILE));
		break;
	default:
		return boost::optional<std::vector<std::string>>();
		break;
	}

	// cap_video_file and picture_file can be set, but empty. therefore, we
	// need to check if the parameter actually contains a file name.
	if (filename && !filename.get().empty()) return filename;
	else return boost::optional<std::vector<std::string>>();
	return boost::optional<std::vector<std::string>>();
}

void BioTracker::exit()
{
	QApplication::exit();
}

void BioTracker::setPlayfieldPaused(bool enabled){
	if(enabled) {
		//video is paused
		ui.button_nextFrame->setEnabled(true);
		ui.button_previousFrame->setEnabled(true);
		ui.button_playPause->setIcon(_iconPlay);
	} else {
		//video is playing
		ui.button_nextFrame->setEnabled(false);
		ui.button_previousFrame->setEnabled(false);
		ui.button_playPause->setIcon(_iconPause);
	}
}

void BioTracker::runCapture()
{	
	switch (_videoMode) {
	case VideoMode::Stopped:
		initPlayback();
		_videoMode = VideoMode::Playing;
		setPlayfieldPaused(false);
		emit videoPause(false);
		break;
	case VideoMode::Paused:
		_videoMode = VideoMode::Playing;
		setPlayfieldPaused(false);
		emit videoPause(false);
		break;
	case VideoMode::Playing:
		pauseCapture();
		break;
	}
}

void BioTracker::invalidFile()
{
	setPlayfieldEnabled(false);
	_videoMode = VideoMode::Stopped;
}

void BioTracker::setPlayfieldEnabled(bool enabled)
{
	ui.button_playPause->setEnabled(enabled);
	ui.button_nextFrame->setEnabled(enabled);
	ui.button_screenshot->setEnabled(enabled);
	ui.button_previousFrame->setEnabled(enabled);
	ui.button_stop->setEnabled(enabled);
}

void BioTracker::closeEvent(QCloseEvent* /* event */)
{
	{
		QFile file(QString::fromStdString(CONFIGPARAM::GEOMETRY_FILE));
		if (file.open(QIODevice::WriteOnly)) file.write(saveGeometry());
	}
	{
		QFile file(QString::fromStdString(CONFIGPARAM::STATE_FILE));
		if (file.open(QIODevice::WriteOnly)) file.write(saveState());
	}
}

boost::optional<BioTracker::filehash> BioTracker::getFileHash(const std::string &filename, const size_t numFiles) const
{
	QCryptographicHash sha1Generator(QCryptographicHash::Sha1);
	QFile file(QString::fromStdString(filename));
	if (file.open(QIODevice::ReadOnly)) {
		// calculate hash from first 4096 bytes of file
		sha1Generator.addData(file.peek(4096));
		sha1Generator.addData(QByteArray::number(static_cast<qulonglong>(numFiles)));
		return QString(sha1Generator.result().toHex()).toStdString();
	}

	return boost::optional<filehash>();
}

std::vector<std::string> BioTracker::getFilenamesFromPaths(const std::vector<std::string> &paths) const
{
	std::vector<std::string> filenames;
	filenames.reserve(paths.size());
	for (std::string const& path : paths) {
		const QFileInfo fi(QString::fromStdString(path));
		filenames.push_back(fi.baseName().toStdString());
	}
	return filenames;
}


bool BioTracker::event(QEvent *event)
{
	if (!_tracker) return QMainWindow::event(event);

	const QEvent::Type etype = event->type();
	if (etype == QEvent::MouseButtonPress ||
	    etype == QEvent::MouseButtonRelease ||
	    etype == QEvent::MouseMove ||
	    etype == QEvent::Wheel)
	{
		if (ui.videoView->rect().contains(ui.videoView->mapFromGlobal(QCursor::pos()))) {
			QCoreApplication::sendEvent(_tracker.get(), event);
			return true;
		}
	} else if (etype == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		const std::set<Qt::Key>& keys = _tracker->grabbedKeys();
		if (keys.count(static_cast<Qt::Key>(keyEvent->key()))) {
			QCoreApplication::sendEvent(_tracker.get(), event);
			return true;
		}
	}
	return QMainWindow::event(event);
}

void BioTracker::stepCaptureForward()
{
	_videoMode = VideoMode::Paused;
	emit grabNextFrame();
	_settings.setParam(GUIPARAM::PAUSED_AT_FRAME, QString::number(_currentFrame).toStdString());
}

void BioTracker::stepCaptureBackward()
{
	if(_currentFrame > 0)
	{
		updateFrameNumber(static_cast<int>(_currentFrame-1));
		emit changeFrame(static_cast<int>(_currentFrame));		
		_settings.setParam(GUIPARAM::PAUSED_AT_FRAME, QString::number(_currentFrame).toStdString());
	}
}

void BioTracker::pauseCapture()
{
	_videoMode = VideoMode::Paused;
	_trackingThread->enableVideoPause(true);
	setPlayfieldPaused(true);
	_settings.setParam(GUIPARAM::PAUSED_AT_FRAME, QString::number(_currentFrame).toStdString());
}

void BioTracker::stopCapture()
{	
	_trackingThread->stopCapture();

	if (_mediaType != MediaType::NoMedia) {
		updateFrameNumber(0);
		_trackingThread->setFrameNumber(0);
	}
	_videoMode = VideoMode::Stopped;

	_settings.setParam(GUIPARAM::PAUSED_AT_FRAME, QString::number(_currentFrame).toStdString());
	setPlayfieldPaused(true);
	ui.sld_video->setDisabled(true);
	ui.cb_algorithms->setCurrentIndex(0);
	trackingAlgChanged(Algorithms::NoTracking);
}

void BioTracker::updateFrameNumber(int frameNumber)
{
	if (_videoMode != VideoMode::Stopped) {
		_currentFrame = frameNumber;

		ui.sld_video->setValue(static_cast<int>(_currentFrame));
		ui.frame_num_edit->setText(QString::number(_currentFrame));
		// check if we reached the end of video
		if (frameNumber == ui.sld_video->maximum()) {
			pauseCapture();
		}
		else if (_videoMode == VideoMode::Paused) {
			_settings.setParam(GUIPARAM::PAUSED_AT_FRAME, QString::number(_currentFrame).toStdString());
		}
	}
}

void BioTracker::drawImage(cv::Mat image)
{
	if(image.data)
	{	
		ui.videoView->showImage(image);
	}

	// signals when the frame was drawn, and the TrackerThread can continue to work;
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
		break;
	default:
		msgLine += " ";
		break;
	}
	msgLine.append(QString::fromStdString(message));
	ui.edit_notification->append(msgLine);
}

void BioTracker::changeCurrentFramebySlider()
{	
	const int value = ui.sld_video->value();
	emit changeFrame(value);
	updateFrameNumber(value);
	_settings.setParam(GUIPARAM::PAUSED_AT_FRAME, QString::number(_currentFrame).toStdString());

}
void BioTracker::changeCurrentFramebySlider(int SliderAction)
{
	int fNum = static_cast<int>(_currentFrame);
	switch(SliderAction)
	{
	case QSlider::SliderPageStepAdd:
		fNum += ui.sld_video->pageStep();
		if (fNum > ui.sld_video->maximum())
			fNum = ui.sld_video->maximum();
		changeCurrentFramebySlider();
		break;
	case QSlider::SliderPageStepSub:
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
	const QString valueStr = ui.frame_num_edit->text();
	//check if string is a number by using regular expression
	QRegExp re("\\d*");  // a digit (\d), zero or more times (*)
	if(re.exactMatch(valueStr))
	{
		int value = valueStr.toInt();
		if(_trackingThread->isReadyForNextFrame())
			emit changeFrame(value);
		_settings.setParam(GUIPARAM::PAUSED_AT_FRAME, QString::number(_currentFrame).toStdString());
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
		emit fpsChange(static_cast<double>(fps));
	}
}

void BioTracker::trackingAlgChanged(QString trackingAlgStr)
{
	trackingAlgChanged(Algorithms::Registry::getInstance().typeByString().at(trackingAlgStr.toStdString()));
}

void BioTracker::trackingAlgChanged(Algorithms::Type trackingAlg)
{
	// calculate file hash of currently opened file
	const boost::optional<std::vector<std::string>> openFiles = getOpenFiles();
	boost::optional<filehash> filehash;
	if (openFiles) filehash = getFileHash(openFiles.get().front(), openFiles.get().size());

	if(_tracker)
	{
		// remove ui containers of old algorithm
		_vboxParams->removeWidget(_paramsWidget.get());
		_vboxTools->removeWidget(_toolsWidget.get());
		_paramsWidget.reset();
		_toolsWidget.reset();

		if (filehash && (_tracker->getType().get() != Algorithms::NoTracking)) {
			// map_type_hashtemp_t maps tracker type to a map that maps
			// a file hash to a QTemporaryFile. We try to find a QTemporaryFile
			// that has been previously created that contains the serialization
			// data of the current tracking algorithm and the currently opened
			// file. If such a QTemporaryFile does not exist yet, we create it
			// now.
			map_hash_temp_t& hashTempMap = _serializationTmpFileMap[_tracker->getType().get()];
			QTemporaryFile& tmpFile = hashTempMap[filehash.get()];
			if (!tmpFile.open()) {
				printGuiMessage("Unable to create temporary file for serialization data!", MSGS::FAIL);
			} else {
				// store the tracking data of the previously selected algorithm
				// in the temporary file.
				storeTrackingData(tmpFile.fileName().toStdString());
			}
		}
		_tracker.reset();
	}

	if (trackingAlg != Algorithms::NoTracking)
	{
		_tracker = Algorithms::Registry::getInstance().make_new_tracker(trackingAlg, _settings, this);
		assert(_tracker);

		// init tracking Alg
		_tracker->setCurrentFrameNumber(static_cast<int>(_currentFrame));
		_tracker->setVideoPaused(_videoMode == VideoMode::Paused);
		connectTrackingAlg(_tracker);

		// now we try to find a temporary file that contains previously
		// stored tracking data for the new tracking algorithm and the
		// currently opened file.
		if (filehash) {
			const auto& hash_tmp_it = _serializationTmpFileMap.find(trackingAlg);
			if (hash_tmp_it != _serializationTmpFileMap.end()) {
				map_hash_temp_t& hashTempMap = (*hash_tmp_it).second;
				// find the previously temporary file
				const auto& tmpfile_it = hashTempMap.find(filehash.get());
				if (tmpfile_it != hashTempMap.end()) {
					QTemporaryFile& file = (*tmpfile_it).second;
					// open file and restore tracking data
					if (!file.open()) printGuiMessage("Unable to load temporary file with serialization data!", MSGS::FAIL);
					loadTrackingData(file.fileName().toStdString());
				}
			}
		}
	}

	ui.groupBox_params->repaint();
	ui.groupBox_tools->repaint();
	emit changeTrackingAlg(_tracker);
}

void BioTracker::connectTrackingAlg(std::shared_ptr<TrackingAlgorithm> tracker)
{	
	if(_tracker)
	{
		QObject::connect(tracker.get(), SIGNAL(notifyGUI(std::string, MSGS::MTYPE)),
			this, SLOT(printGuiMessage(std::string, MSGS::MTYPE)));
		QObject::connect( tracker.get(), SIGNAL( update() ),
			ui.videoView, SLOT( updateGL() ));
		QObject::connect(tracker.get(), SIGNAL( forceTracking() ),
			_trackingThread.get(), SLOT( doTrackingAndUpdateScreen() ));
		QObject::connect(_trackingThread.get(), SIGNAL( newFrameNumber(int) ),
			tracker.get(), SLOT( setCurrentFrameNumber(int) ));
		QObject::connect(this, SIGNAL( videoPause(bool) ),
			tracker.get(), SLOT(setVideoPaused(bool)));
		QObject::connect(ui.videoView, SIGNAL(reportZoomLevel(float)),
			tracker.get(), SLOT(setZoomLevel(float)));
		QObject::connect(_trackingThread.get(), &TrackingThread::trackingSequenceDone,
						 _tracker.get(), &TrackingAlgorithm::setCurrentImage);

		_tracker->setZoomLevel(ui.videoView->getCurrentZoomLevel());
		try
		{
			_paramsWidget = _tracker->getParamsWidget();
			_vboxParams->addWidget(_paramsWidget.get());
			_toolsWidget = _tracker->getToolsWidget();
			_vboxTools->addWidget(_toolsWidget.get());
		}
		catch(std::exception&)
		{
			emit printGuiMessage("cannot create UI elements for selected algorithm", MSGS::FAIL);
		}
	}
	ui.videoView->update();
}

void BioTracker::takeScreenshot()
{
	const std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
	const std::time_t t = std::chrono::system_clock::to_time_t(p);
	std::string dateTime = std::ctime(&t);
	// ctime adds a newline to the string due to historical reasons
	dateTime = dateTime.substr(0, dateTime.size() - 1);
	QString filepath = QString::fromStdString(_settings.getValueOrDefault<std::string>(CAPTUREPARAM::CAP_SCREENSHOT_PATH, "."));
	filepath.append("/screenshot_").append(QString::fromStdString(dateTime)).append(".png");
	ui.videoView->takeScreenshot(filepath);
}

void BioTracker::switchPanZoomMode()
{
	_isPanZoomMode = !_isPanZoomMode;
	ui.videoView->setPanZoomMode(_isPanZoomMode);
}

void BioTracker::displayFileName(QString& filename)
{
	ui.lbl_filename->setText(filename);
}
