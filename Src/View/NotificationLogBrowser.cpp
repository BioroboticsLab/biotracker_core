#include "NotificationLogBrowser.h"
#include "QVBoxLayout"
#include "QScrollBar"
#include "QMutexLocker"


NotificationLogBrowser::NotificationLogBrowser(QWidget *parent, IController *controller, IModel *model) :
	IViewWidget(parent, controller, model)
{
	QMutex m_mutex;
	QVBoxLayout *layout = new QVBoxLayout(this);

	browser = new QTextEdit(this);
	browser->setReadOnly(true);
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

void NotificationLogBrowser::outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QMutexLocker locker(&m_mutex);
	switch (type) {
		case QtDebugMsg:
		case QtInfoMsg:
			browser->append(msg);
			break;

		case QtWarningMsg:
			browser->append(tr("<span style='color:orange;'>- %1 line: %2</span>").arg(msg).arg(context.line));
			break;

		case QtCriticalMsg:
			browser->append(tr("<span style='color:red;'>- %1 line: %2</span>").arg(msg).arg(context.line));
			break;

		case QtFatalMsg:
			browser->append(tr("<span style='color:red; font-weight:bold'>- %1 line: %2</span>").arg(msg).arg(context.line));
			break;
		
		default:
			browser->append(msg);
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
