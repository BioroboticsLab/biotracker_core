#include "BioTracker.h"
#include <QtWidgets/QApplication>
#include "settings/Settings.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	if (!QGLFormat::hasOpenGL()) {
        std::cerr << "This system has no OpenGL support" << std::endl;
        return 1;
    }
	Settings settings;

	app.setOrganizationName("Biorobotics Lab / FU Berlin");
	app.setApplicationName("Bio Tracker");
	
	BioTracker w(settings);
	w.show();
	return app.exec();
}
