#include <QtWidgets/QApplication>

#include "source/gui/BioTracker.h"
#include "source/settings/Messages.h"
#include "source/settings/Settings.h"
#include "source/settings/SystemCompatibilityCheck.h"
#include "source/utility/stdext.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	//meta types
	qRegisterMetaType<cv::Mat>("cv::Mat");
	qRegisterMetaType<MSGS::MTYPE>("MSGS::MTYPE");
	qRegisterMetaType<std::string>("std::string");

	if (SystemCompatibilityCheck::checkOpenGLSupport()) {
		Settings settings;

		app.setOrganizationName("Biorobotics Lab / FU Berlin");
		app.setApplicationName("BioTracker");

		BioTracker w(settings);
		w.show();
		return app.exec();
	} else {
		static const std::string title = MSGS::SYSTEM::APPLICATION_CANNOT_START;
		static const std::string msg   = MSGS::SYSTEM::NO_OPENGL;
		QMessageBox::critical(nullptr,
		                      QString::fromStdString(title),
		                      QString::fromStdString(msg));
	}
	return EXIT_FAILURE;
}
