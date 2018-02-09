#include "ControllerNotifications.h"
#include "View/NotificationLogBrowser.h"
#include "ControllerMainWindow.h"
#include "qdebug.h"

IView*  view;

ControllerNotifications::ControllerNotifications(QObject* parent, IBioTrackerContext* context, ENUMS::CONTROLLERTYPE ctr) :
	IController(parent, context, ctr) {

}

ControllerNotifications::~ControllerNotifications() {
    qInstallMessageHandler(0);
}

void ControllerNotifications::cleanup() {
    qInstallMessageHandler(0);
}

void ControllerNotifications::createModel()
{
}

void ControllerNotifications::createView()
{
	m_View = new NotificationLogBrowser();
	view = m_View;
	qInstallMessageHandler(messageHandler);

}

void ControllerNotifications::connectModelToController()
{
}

void ControllerNotifications::connectControllerToController()
{
	// Add notification browser to Main Window
	IController* ctrA = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
	QPointer< ControllerMainWindow > ctrMainWindow = qobject_cast<ControllerMainWindow*>(ctrA);

	ctrMainWindow->setNotificationBrowserWidget(m_View);
}



void messageHandler(QtMsgType type, const QMessageLogContext & context, const QString & msg)
{
	NotificationLogBrowser* log = dynamic_cast<NotificationLogBrowser*>(view);
	if (log) {
		log->outputMessage(type, msg);
	}
}
