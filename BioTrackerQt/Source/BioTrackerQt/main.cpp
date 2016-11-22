#include <QApplication>
#include "BioTracker3App.h"
#include "GuiContext.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    /// NOTE edit by Andi

    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<std::size_t>("std::size_t");
    qRegisterMetaType<size_t>("size_t");
    qRegisterMetaType<std::vector<boost::filesystem::path>>("std::vector<boost::filesystem::path>");
    qRegisterMetaType<QVector<bool>>("QVector<bool>");



    //BioTrackerController mController;

    BioTracker3App bioTracker3(&app);
    GuiContext context(&bioTracker3);
    bioTracker3.setBioTrackerContext(&context);
    bioTracker3.runBioTracker();

    app.exec();
}
