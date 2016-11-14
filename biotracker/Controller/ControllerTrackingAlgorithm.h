#ifndef CONTROLLERTRACKINGALGORITHM_H
#define CONTROLLERTRACKINGALGORITHM_H

#include "Interfaces/icontroller.h"
#include "Model/BioTrackerTrackingAlgorithm.h"

class ControllerTrackingAlgorithm : public IController
{
    Q_OBJECT
public:
    ControllerTrackingAlgorithm(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);

    // IController interface
public:
    void connectViewToMainWindow(IController *controller) override;
    void connectToOtherController(IController *controller) override;

    void callAnOtherController() override;

protected:
    void createModel() override;
    void createView() override;
    void connectModelController() override;
    void connectModelView() override;

Q_SIGNALS:
    void emitCvMatA(cv::Mat mat, QString name);

public Q_SLOTS:
    void receiveCvMatA(cv::Mat mat, QString name);
};

#endif // CONTROLLERTRACKINGALGORITHM_H
