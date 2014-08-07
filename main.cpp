#include "BioTracker.h"
#include <QtWidgets/QApplication>
#include "settings/Settings.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Settings settings;

	a.setOrganizationName("Biorobotics Lab / FU Berlin");
	a.setApplicationName("Bio Tracker");
	
	BioTracker w(settings);
	w.show();
	return a.exec();
}
