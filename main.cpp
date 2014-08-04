#include "BioTracker.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BioTracker w;
	w.show();
	return a.exec();
}
