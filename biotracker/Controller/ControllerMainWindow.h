#ifndef CONTROLLERMAINWINDOW_H
#define CONTROLLERMAINWINDOW_H

#include "Interfaces/icontroller.h"

class ControllerMainWindow : public IController
{
    Q_OBJECT
public:
    ControllerMainWindow(QObject *parent = 0, IBioTrackerContext *context = 0);

    void loadVideo(QString str);

    // IController interface
public:
    void connectViewToMainWindow(IController *controller) override;

protected:
    void createModel() override;
    void createView() override;

    // IController interface
protected:
    void connectModelController() override;
    void connectModelView() override;
};

#endif // CONTROLLERMAINWINDOW_H
