#include "BioTrackerController.h"
#include "BioTrackerTrackingAlgorithm.h"

#include "../biotracker_gui/biotracker/BioTracker3VideoControllWidget.h"
#include "../biotracker_gui/biotracker/BioTracker3VideoView.h"


BioTrackerController::BioTrackerController() :
    m_imageStream(BioTracker::Core::make_ImageStreamNoMedia()) {   // ImageStream hast to be setup with NOMedia
    // initialize Attribute
    m_texture = new BioTracker::Core::TextureObject();
    m_VideoView = new BioTracker3VideoView(0,0);

    // create Views and Userinput
    createBioTrackerMainWindow();
    createVideoControllWidget();
    createVideoViewWidget();

    // create Model
    createBioTrackingTrackingAlgorithm();
}

void BioTrackerController::loadVideo(QString str) {
    // Save the path to the video file in Settings
    m_Settings.setParam(CaptureParam::CAP_VIDEO_FILE, boost::filesystem::path(str.toStdString()));

    // create the ImageStream for the Vidoe
    createVideoStream();
}

void BioTrackerController::startTracking() {
    BioTrackerTrackingAlgorithm *tracker = dynamic_cast<BioTrackerTrackingAlgorithm *>(getModel(0));

    //mat = m_imageStream->currentFrame();
    //tracker->track(mat);

    // delete
    showImageInVideoView();
}

void BioTrackerController::showImageInVideoView() {
    //m_imageStream->nextFrame();
    m_texture->set(m_imageStream->currentFrame());

    BioTracker3VideoView *view = getVideoView();
}

BioTracker3VideoView *BioTrackerController::getVideoView() {
    return m_VideoView;
}

void BioTrackerController::setVideoView(BioTracker3VideoView *videoview) {
    m_VideoView = videoview;
}

void BioTrackerController::createVideoStream() {
    std::string filenameStr = m_Settings.getValueOfParam<std::string>(CaptureParam::CAP_VIDEO_FILE);
    // TODO trim the string in Settings::getValueOfParam
    // There is a " char at the beginning and the end of the string, so they have to be removed.
    filenameStr.erase(filenameStr.begin());
    filenameStr.erase(filenameStr.end()-1);

    boost::filesystem::path filename {filenameStr};
    m_imageStream = BioTracker::Core::make_ImageStreamVideo(filename);
}

void BioTrackerController::createVideoViewWidget() {
    BioTracker3MainWindow *mainWindow = getMainWindow();

    BioTracker3VideoView *videoView = new BioTracker3VideoView(this, m_texture);

    mainWindow->addVideoView(videoView);

    //addView(videoView);
}


void BioTrackerController::createVideoControllWidget() {
    // Setup VideoControllWidget
    BioTracker3VideoControllWidget *videoControlWidget = new BioTracker3VideoControllWidget(this, 0);

    BioTracker3MainWindow *mainWindow = getMainWindow();

    mainWindow->addVideoControllWidget(videoControlWidget);


    addView(videoControlWidget);

}

void BioTrackerController::createBioTrackingTrackingAlgorithm() {
    BioTrackerTrackingAlgorithm *algorithm = new BioTrackerTrackingAlgorithm();

    // set Model to View
    IView *view = getView(0);
    view->setModel(algorithm);

    // add Model to IController
    addModel(algorithm);
}

void BioTrackerController::createBioTrackerMainWindow() {
    BioTracker3MainWindow *mainWindow = new BioTracker3MainWindow();
    mainWindow->addController(this);
    registerMainBioTrackerWindow(mainWindow);
    mainWindow->show();
}
