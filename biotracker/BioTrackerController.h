#ifndef BIOTRACKERCONTROLLER_H
#define BIOTRACKERCONTROLLER_H
#include "Interfaces/icontroller.h"
#include "settings/Settings.h"
#include "ImageStream.h"
#include <memory>
#include "TextureObject.h"

class BioTrackerController : public IController {
  public:
    BioTrackerController();

    // User Input
    void loadVideo(QString str);
    void startTracking();
    void showImageInVideoView();

    // change
    BioTracker3VideoView *getVideoView();
    void setVideoView(BioTracker3VideoView *videoview);

  private:

    // create Views and User Input
    void createVideoControllWidget();
    void createBioTrackerMainWindow();

    // create Models like TrackingAlgorithm
    void createBioTrackingTrackingAlgorithm();
    void createVideoStream();

    // create VideoView
    void createVideoViewWidget();



  private:
    BioTracker::Core::Settings m_Settings;
    std::unique_ptr<BioTracker::Core::ImageStream> m_imageStream;

    // This is the Object that contains the current Image
    BioTracker::Core::TextureObject *m_texture;

    // change
    BioTracker3VideoView *m_VideoView;
};

#endif // BIOTRACKERCONTROLLER_H
