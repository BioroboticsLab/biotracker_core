#ifndef CONTROLLERTRACKINGALGORITHM_H
#define CONTROLLERTRACKINGALGORITHM_H

#include "Interfaces/IController/IController.h"
#include "Model/BioTrackerTrackingAlgorithm.h"
#include "Interfaces/IBioTrackerContext.h"


class ControllerTrackingAlgorithm : public IController
{
    Q_OBJECT
public:
    ControllerTrackingAlgorithm(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);

    // IController interface
public:
    void connectControllerToController() override;

    void doTracking(std::shared_ptr<cv::Mat> mat, uint number);

    IView *getTrackingParameterWidget();

protected:
    void createModel() override;
    void createView() override;
    void connectModelToController() override;

Q_SIGNALS:
    void emitCvMat(std::shared_ptr<cv::Mat> mat, QString name);
    void emitTrackingDone();

private Q_SLOTS:
    void receiveCvMatFromTrackingAlgorithm(std::shared_ptr<cv::Mat> mat, QString name);
    void receiveTrackingDone();


private:
    IModel* m_TrackingParameter;
};

#endif // CONTROLLERTRACKINGALGORITHM_H
