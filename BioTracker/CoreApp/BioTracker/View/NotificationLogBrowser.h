#ifndef NOTIFICATIONLOGBROWSER_H
#define NOTIFICATIONLOGBROWSER_H
#pragma once

#include "Interfaces/IView/IViewWidget.h"
#include <QTextBrowser>

class NotificationLogBrowser : public IViewWidget
{
	Q_OBJECT

	public:
		explicit NotificationLogBrowser(QWidget *parent = 0, IController *controller = 0, IModel *model = 0);
		~NotificationLogBrowser();

		void outputMessage(QtMsgType type, const QString &msg);

	// IViewWidget interface
	public slots:
		void getNotified();

	private:
		QTextBrowser *browser;
		QFont m_font;

};

#endif //NOTIFICATIONLOGBROWSER_H