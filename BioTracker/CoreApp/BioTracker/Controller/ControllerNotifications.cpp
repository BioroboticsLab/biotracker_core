#include "ControllerNotifications.h"
#include "View/NotificationLogBrowser.h"
#include "qDebug"

IView*  view;

ControllerNotifications::ControllerNotifications(QObject* parent, IBioTrackerContext* context, ENUMS::CONTROLLERTYPE ctr) :
	IController(parent, context, ctr) {

}

void ControllerNotifications::createModel()
{
}

void ControllerNotifications::createView()
{
	m_View = new NotificationLogBrowser();
	view = m_View;
	qInstallMessageHandler(messageHandler);
	qDebug() << "BLALAAL";

}

void ControllerNotifications::connectModelToController()
{
}

void ControllerNotifications::connectControllerToController()
{
}



void messageHandler(QtMsgType type, const QMessageLogContext & context, const QString & msg)
{
	NotificationLogBrowser* log = dynamic_cast<NotificationLogBrowser*>(view);
	if (log) {
		log->outputMessage(type, msg);
	}
}
