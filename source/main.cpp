#include "source/gui/BioTracker.h"
#include <QtWidgets/QApplication>
#include "source/settings/Settings.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	if (!QGLFormat::hasOpenGL()) {
        std::cerr << "This system has no OpenGL support" << std::endl;
        return 1;
    }
	Settings settings;
    settings.setParam(TRACKERPARAM::TRACKING_ENABLED, "true");
    settings.setParam(CAPTUREPARAM::CAP_VIDEO_FILE, "");

	app.setOrganizationName("Biorobotics Lab / FU Berlin");
	app.setApplicationName("Bio Tracker");
	
	BioTracker w(settings);
	w.show();
	return app.exec();
}
