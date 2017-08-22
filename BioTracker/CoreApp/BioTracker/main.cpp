#include <QApplication>
#include "BioTracker3App.h"
#include "GuiContext.h"
#include "opencv2/core/core.hpp"
#include <boost/filesystem.hpp>
#include "QVector"
#include "Model/MediaPlayerStateMachine/PlayerParameters.h"
#include "util/types.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);


    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<std::shared_ptr<cv::Mat>>("std::shared_ptr<cv::Mat>");
    qRegisterMetaType<std::size_t>("std::size_t");
    qRegisterMetaType<size_t>("size_t");
    qRegisterMetaType<std::vector<boost::filesystem::path>>("std::vector<boost::filesystem::path>");
    qRegisterMetaType<QVector<bool>>("QVector<bool>");
    qRegisterMetaType<playerParameters*>("playerParameters*");
	qRegisterMetaType<CameraConfiguration>("CameraConfiguration");



    //BioTrackerController mController;

    BioTracker3App bioTracker3(&app);
    GuiContext context(&bioTracker3);
    bioTracker3.setBioTrackerContext(&context);
	bioTracker3.runBioTracker();

    app.exec();
}
