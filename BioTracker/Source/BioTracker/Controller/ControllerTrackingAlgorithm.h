#ifndef CONTROLLERTRACKINGALGORITHM_H
#define CONTROLLERTRACKINGALGORITHM_H

#include "Interfaces/IController/IController.h"
#include "Model/BioTrackerTrackingAlgorithm.h"


class ControllerTrackingAlgorithm : public IController
{
    Q_OBJECT
public:
    ControllerTrackingAlgorithm(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);

    // IController interface
public:
    void connectControllerToController() override;

protected:
    void createModel() override;
    void createView() override;
    void connectModelToController() override;

Q_SIGNALS:
    void emitCvMatA(std::shared_ptr<cv::Mat> mat, QString name);

public Q_SLOTS:
    void receiveCvMatA(std::shared_ptr<cv::Mat> mat, QString name);

};

#endif // CONTROLLERTRACKINGALGORITHM_H
