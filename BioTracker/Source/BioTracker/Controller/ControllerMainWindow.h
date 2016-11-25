#ifndef CONTROLLERMAINWINDOW_H
#define CONTROLLERMAINWINDOW_H

#include "Interfaces/IController/icontroller.h"
#include "QPointer"

class ControllerMainWindow : public IController
{
    Q_OBJECT
public:
    ControllerMainWindow(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);

    void loadVideo(QString str);
    void loadTracker(QString str);
    void loadPictures(std::vector<boost::filesystem::path> files);
    void loadCameraDevice(int i);


    // IController interface
protected:
    void createModel() override;
    void createView() override;
    void connectModelController() override;
    void connectController() override;
};

#endif // CONTROLLERMAINWINDOW_H
