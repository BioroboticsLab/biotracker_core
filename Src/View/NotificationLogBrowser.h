#ifndef NOTIFICATIONLOGBROWSER_H
#define NOTIFICATIONLOGBROWSER_H
#pragma once

#include "Interfaces/IView/IViewWidget.h"
#include <QTextEdit>
#include <QMutex>

/**
* This is the view of the notifications component.
* It displays all intercepted qdebug messages (color-coded).
* http://doc.qt.io/qt-5/qdebug.html
*/
class NotificationLogBrowser : public IViewWidget
{
	Q_OBJECT

public:
	explicit NotificationLogBrowser(QWidget *parent = 0, IController *controller = 0, IModel *model = 0);
	~NotificationLogBrowser();

	void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

	// IViewWidget interface
	public slots:
	void getNotified();

private:
	QTextEdit *browser;
	QFont m_font;
	QMutex m_mutex;

};

#endif //NOTIFICATIONLOGBROWSER_H