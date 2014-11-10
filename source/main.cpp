#include <QtWidgets/QApplication>

#include "source/utility/stdext.h"
#include "source/gui/BioTracker.h"
#include "source/settings/Settings.h"
#include "source/settings/Messages.h"
#include "source/settings/SystemCompatibilityCheck.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto notificationMessage = std::make_unique<QString>("No Error!");

    if(SystemCompatibilityCheck::checkAll(notificationMessage.get()))
	{
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

    return EXIT_FAILURE;
}
