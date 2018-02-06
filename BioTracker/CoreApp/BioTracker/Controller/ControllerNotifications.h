#pragma once

#ifndef CONTROLLERNOTIFICATIONS_H
#define CONTROLLERNOTIFICATIONS_H

#include "Interfaces/IController/IController.h"

class ControllerNotifications : public IController {
	Q_OBJECT
	public:
        ControllerNotifications(QObject* parent = 0, IBioTrackerContext* context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);
        ~ControllerNotifications();
		
	protected:
		void createModel() override;
		void createView() override;
		void connectModelToController() override;
		void connectControllerToController() override;

};

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // CONTROLLERNOTIFICATIONS_H