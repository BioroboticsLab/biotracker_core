#include <clocale>

#include <QtWidgets/QApplication>

#include "source/core/settings/Messages.h"
#include "source/core/settings/Settings.h"
#include "source/core/settings/SystemCompatibilityCheck.h"
#include "source/gui/Gui.h"
#include "source/util/stdext.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	// workaround for cereal issue 144
	// see: https://github.com/USCiLab/cereal/issues/144
	std::setlocale(LC_NUMERIC, "C");

	// meta types
	qRegisterMetaType<cv::Mat>("cv::Mat");
	qRegisterMetaType<MSGS::MTYPE>("MSGS::MTYPE");
	qRegisterMetaType<std::string>("std::string");

	if (SystemCompatibilityCheck::checkOpenGLSupport()) {
		Settings settings;

		app.setOrganizationName("Biorobotics Lab / FU Berlin");
		app.setApplicationName("BioTracker");

		Gui w(settings);
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
