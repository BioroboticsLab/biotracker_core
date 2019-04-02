#include "ControllerNotifications.h"
#include "View/NotificationLogBrowser.h"
#include "ControllerMainWindow.h"
#include "qdebug.h"

#include <QFile> 

IView*  view;

ControllerNotifications::ControllerNotifications(QObject* parent, IBioTrackerContext* context, ENUMS::CONTROLLERTYPE ctr) :
	IControllerCfg(parent, context, ctr) {

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

// Get the default Qt message handler.
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(0);

void messageHandler(QtMsgType type, const QMessageLogContext & context, const QString & msg)
{
	//notification text browser
	NotificationLogBrowser* log = dynamic_cast<NotificationLogBrowser*>(view);
	if (log) {
		log->outputMessage(type, msg);
	}

	//log file
	if (type == QtMsgType::QtInfoMsg)
        return;

    QString fmsg = qFormatLogMessage(type, context, msg);
	QString outf = IConfig::dataLocation+"/log.txt";
	std::string debugs = outf.toStdString();
    QFile outFile(outf);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile); 
    ts << fmsg << endl;

    (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
}
