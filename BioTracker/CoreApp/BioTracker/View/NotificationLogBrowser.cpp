#include "NotificationLogBrowser.h"
#include "ui_CoreParameterView.h"


NotificationLogBrowser::NotificationLogBrowser(QWidget *parent, IController *controller, IModel *model) :
	IViewWidget(parent, controller, model)
	//ui(new Ui::NotificationLogBrowser)
{
	//ui->setupUi(this);
	QVBoxLayout *layout = new QVBoxLayout(this);
	setLayout(layout);

	browser = new QTextBrowser(this);
	layout->addWidget(browser);



	resize(200, 400);

	this->show();

}

NotificationLogBrowser::~NotificationLogBrowser()
{
	delete ui;
}

void NotificationLogBrowser::outputMessage(QtMsgType type, const QString & msg)
{
	switch (type) {
	case QtDebugMsg:
		browser->append(msg);
		break;

	case QtWarningMsg:
		browser->append(tr("— WARNING: %1").arg(msg));
		break;

	case QtCriticalMsg:
		browser->append(tr("— CRITICAL: %1").arg(msg));
		break;

	case QtFatalMsg:
		browser->append(tr("— FATAL: %1").arg(msg));
		break;
	}
}
void  NotificationLogBrowser::getNotified() {

}
