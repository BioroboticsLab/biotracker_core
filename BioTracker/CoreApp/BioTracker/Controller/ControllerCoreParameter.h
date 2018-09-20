#pragma once

#include "Interfaces/IController/IController.h"
#include "Interfaces/IModel/IModelAreaDescriptor.h"
#include <QMouseEvent>
#include <QKeyEvent>

class ControllerCoreParameter : public IController
{
	Q_OBJECT
public:
	ControllerCoreParameter(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::COREPARAMETER);

    //Triggers the view to re-send gui parameters
    void triggerUpdate();

signals:
	//...

	private slots:
	//...
	void updateView();
	//update track number
	void receiveTrackNumber(int number);

	public slots:
	void setCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool> permission);
    void changeAreaDescriptorType(QString type);
	void receiveResetTrial();

    //Forwarded from data exporter
    int getTrialNumber();

	// IController interface
protected:
	void createModel() override;
	void createView() override;
	void connectModelToController() override;
	void connectControllerToController() override;
private:
	//...

};
