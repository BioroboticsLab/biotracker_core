#include "NotificationLogBrowser.h"
#include "QVBoxLayout"


NotificationLogBrowser::NotificationLogBrowser(QWidget *parent, IController *controller, IModel *model) :
	IViewWidget(parent, controller, model)
	//ui(new Ui::NotificationLogBrowser)
{
	//ui->setupUi(this);
	QVBoxLayout *layout = new QVBoxLayout(this);

	browser = new QTextBrowser(this);
	browser->setLineWrapMode(QTextEdit::NoWrap);

	layout->addWidget(browser);

	setLayout(layout);

	m_font = QFont();
	m_font.setPointSize(8);
	setFont(m_font);

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
		browser->append(tr("<span style='color:orange;'>- %1 </span>").arg(msg));
		break;

	case QtCriticalMsg:
		browser->append(tr("<span style='color:red;'>- %1 </span>").arg(msg));
		break;

	case QtFatalMsg:
		browser->append(tr("<span style='color:red; font-weight:bold'>- %1 </span>").arg(msg));
		break;
	}
}
void  NotificationLogBrowser::getNotified() {

}
