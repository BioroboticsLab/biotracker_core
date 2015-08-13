#include <clocale>

#include <QApplication>
#include <QMessageBox>

#include "biotracker/core/settings/Messages.h"
#include "biotracker/core/settings/Settings.h"
#include "biotracker/core/settings/SystemCompatibilityCheck.h"
#include "biotracker/gui/Gui.h"
#include "biotracker/util/stdext.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // workaround for cereal issue 144
    // see: https://github.com/USCiLab/cereal/issues/144
    std::setlocale(LC_NUMERIC, "C");

    // meta types
    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<MSGS::MTYPE>("MSGS::MTYPE");
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<std::size_t>("std::size_t");
    qRegisterMetaType<size_t>("size_t");
    qRegisterMetaType<BioTracker::Core::TrackerType>("TrackerType");

    if (SystemCompatibilityCheck::checkOpenGLSupport()) {
        app.setOrganizationName("Biorobotics Lab / FU Berlin");
        app.setApplicationName("BioTracker");

        BioTracker::Gui::Gui w;
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
