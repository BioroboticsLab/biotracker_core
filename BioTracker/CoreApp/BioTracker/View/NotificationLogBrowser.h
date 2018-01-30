#ifndef NOTIFICATIONLOGBROWSER_H
#define NOTIFICATIONLOGBROWSER_H
#pragma once

#include "Interfaces/IView/IViewWidget.h"
#include <QTextBrowser>

namespace Ui {
	class NotificationLogBrowser;
}

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
		Ui::NotificationLogBrowser *ui;
		QTextBrowser *browser;

};

#endif //NOTIFICATIONLOGBROWSER_H