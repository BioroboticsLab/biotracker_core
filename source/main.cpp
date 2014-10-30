#include <QtWidgets/QApplication>

#include "source/gui/BioTracker.h"
#include "source/settings/Settings.h"
#include "source/settings/Messages.h"
#include "source/helper/SystemConfigCheck.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QString* notificationMessage = new QString("No Error!");

	SystemConfigCheck sysConfChk;

	if(sysConfChk.checkAll(notificationMessage))
	{
		delete notificationMessage;

		Settings settings;

		app.setOrganizationName("Biorobotics Lab / FU Berlin");
		app.setApplicationName("Bio Tracker");
	
		BioTracker w(settings);
		w.show();
		return app.exec();
	} else 
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle(QString::fromStdString(MSGS::SYSTEM::APPLICATION_CANNOT_START));
		msgBox.setText(*notificationMessage);
		msgBox.setStandardButtons(QMessageBox::Ok);		
		msgBox.exec();
	}

	delete notificationMessage;
	return EXIT_FAILURE;
}
