#pragma once

#ifndef CONTROLLERNOTIFICATIONS_H
#define CONTROLLERNOTIFICATIONS_H

#include "IControllerCfg.h"

/**
* This class inherits from IController.
* The notifications component includes the notifications view.
* This controller uses qInstallMessageHandler(messageHandler) to
* intercept all qDebug messages and display them in a text browser (the view).
* This text browser is displayed in the 'Notifications' tab in the GUI.
* QDebug reference: http://doc.qt.io/qt-5/qdebug.html
*/
class ControllerNotifications : public IControllerCfg {
	Q_OBJECT
public:
	ControllerNotifications(QObject* parent = 0, IBioTrackerContext* context = 0,
		ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);
	~ControllerNotifications();
	void cleanup() override;

protected:
	void createModel() override;
	void createView() override;
	void connectModelToController() override;
	void connectControllerToController() override;

};

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // CONTROLLERNOTIFICATIONS_H