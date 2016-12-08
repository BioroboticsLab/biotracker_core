#ifndef CONTROLLERMAINWINDOW_H
#define CONTROLLERMAINWINDOW_H

#include "Interfaces/IController/IController.h"
#include "QPointer"
#include "QStringListModel"

class ControllerMainWindow : public IController
{
    Q_OBJECT
public:
    ControllerMainWindow(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);

    void loadVideo(QString str);
    void loadTracker(QString str);
    void loadPictures(std::vector<boost::filesystem::path> files);
    void loadCameraDevice(int i);
    void activeTracking();
    void deactiveTrackring();
    void setTrackerList(QStringListModel *trackerList);


    // IController interface
protected:
    void createModel() override;
    void createView() override;
    void connectModelToController() override;
    void connectControllerToController() override;
};

#endif // CONTROLLERMAINWINDOW_H
