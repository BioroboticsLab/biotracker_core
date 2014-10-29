#include "source/gui/BioTracker.h"
#include <QtWidgets/QApplication>
#include "source/settings/Settings.h"

int main(int argc, char *argv[])
{
//    std::ofstream osxml("out.xml", std::ios::binary);
//    cereal::XMLOutputArchive archivexml(osxml);

//    std::ofstream osjson("out.json", std::ios::binary);
//    cereal::JSONOutputArchive archivejson(osjson);

    //TrackedObject object(0);
    //std::unique_ptr<ObjectModel> model = std::unique_ptr<ObjectModel>(new DerivedObjectModel());
    //archivexml(object);
    //archivejson(object);

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
