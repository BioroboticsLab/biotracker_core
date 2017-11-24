#ifndef CONTROLLERTRACKINGALGORITHM_H
#define CONTROLLERTRACKINGALGORITHM_H

#include "Interfaces/IController/IController.h"
#include "Model/BioTrackerTrackingAlgorithm.h"
#include "Interfaces/IBioTrackerContext.h"
#include "Interfaces/IModel/IModelDataExporter.h"


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

public Q_SLOTS:
	void receiveAreaDescriptorUpdate(IModelAreaDescriptor *areaDescr);

protected:
    void createModel() override;
    void createView() override;
    void connectModelToController() override;

Q_SIGNALS:
    void emitCvMat(std::shared_ptr<cv::Mat> mat, QString name);
    void emitTrackingDone(uint framenumber);
	void emitChangeDisplayImage(QString str);
	void emitAreaDescriptorUpdate(IModelAreaDescriptor *areaDescr);

private Q_SLOTS:
    void receiveCvMatFromTrackingAlgorithm(std::shared_ptr<cv::Mat> mat, QString name);
    void receiveTrackingDone(uint framenumber);
	void receiveChangeDisplayImage(QString str);

private:
    IModel* m_TrackingParameter;
	IModel *m_TrackedTrajectoryMajor;
};

#endif // CONTROLLERTRACKINGALGORITHM_H
