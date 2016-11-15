#ifndef CONTROLLERMAINWINDOW_H
#define CONTROLLERMAINWINDOW_H

#include "Interfaces/icontroller.h"

class ControllerMainWindow : public IController
{
    Q_OBJECT
public:
    ControllerMainWindow(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);

    void loadVideo(QString str);


    // IController interface
protected:
    void createModel() override;
    void createView() override;
    void connectModelController() override;
    void connectController() override;
};

#endif // CONTROLLERMAINWINDOW_H
