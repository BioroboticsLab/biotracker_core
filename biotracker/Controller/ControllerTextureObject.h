#ifndef CONTROLLERTEXTUREOBJECT_H
#define CONTROLLERTEXTUREOBJECT_H

#include "Interfaces/icontroller.h"

class ControllerTextureObject : public IController
{
    Q_OBJECT
public:
    ControllerTextureObject(QObject *parent = 0, IBioTrackerContext *context = 0);

    // IController interface
public:
    void connectViewToMainWindow(IController *controller) override;
    void connectToOtherController(IController *controller) override;

public Q_SLOTS:
    void receiveCvMat(cv::Mat mat);

protected:
    void createModel() override;
    void createView() override;
    void connectModelController() override;
    void connectModelView() override;

};

#endif // CONTROLLERTEXTUREOBJECT_H
