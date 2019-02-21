#include "NotificationLogBrowser.h"
#include "QVBoxLayout"
#include "QScrollBar"


NotificationLogBrowser::NotificationLogBrowser(QWidget *parent, IController *controller, IModel *model) :
	IViewWidget(parent, controller, model)
{
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

	QScrollBar *sb = browser->verticalScrollBar();
	int value = sb->value();
	int max = sb->maximum();
	if(max - value <= 15){
		sb->setValue(sb->maximum());
	}
}
void  NotificationLogBrowser::getNotified() {

}
