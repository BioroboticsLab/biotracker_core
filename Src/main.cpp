#include <QApplication>
#include "BioTracker3App.h"
#include "GuiContext.h"
#include "opencv2/core/core.hpp"
#include <boost/filesystem.hpp>
#include <QVector>
#include <QStandardPaths>
#include <QTextStream>
#include <QFile> 
#include "Model/MediaPlayerStateMachine/PlayerParameters.h"
#include "util/types.h"
#include "util/camera/base.h"

#include "util/CLIcommands.h"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include "util/Config.h"
#include <QDir>

//This will hide the console. 
//See https://stackoverflow.com/questions/2139637/hide-console-of-windows-application
#ifdef _WIN32
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

// Get the default Qt message handler.
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(0);

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (type == QtMsgType::QtInfoMsg)
        return;

    QString fmsg = qFormatLogMessage(type, context, msg);
	QString outf = IConfig::dataLocation+"/log.txt";
	std::string debugs = outf.toStdString();
    QFile outFile(outf);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile); 
    ts << fmsg << endl;

    (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    app.setOrganizationName("FU Berlin");
    app.setApplicationName("BioTracker");
    IConfig::configLocation = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    IConfig::dataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    Config *cfg = new Config();
	CLI::optionParser(argc, argv, cfg);
    QString cfgLoc = cfg->CfgCustomLocation.isEmpty()?Config::configLocation:cfg->CfgCustomLocation;
    cfg->load(cfgLoc, "config.ini");
    cfg->save(cfgLoc, "config.ini");

    qRegisterMetaType<cv::Mat>("cv::Mat");
    qRegisterMetaType<std::shared_ptr<cv::Mat>>("std::shared_ptr<cv::Mat>");
    qRegisterMetaType<std::size_t>("std::size_t");
    qRegisterMetaType<size_t>("size_t");
    qRegisterMetaType<std::vector<boost::filesystem::path>>("std::vector<boost::filesystem::path>");
    qRegisterMetaType<BiotrackerTypes::AreaType>("BiotrackerTypes::AreaType");
    qRegisterMetaType<QVector<bool>>("QVector<bool>");
    qRegisterMetaType<playerParameters*>("playerParameters*");
	qRegisterMetaType<CameraConfiguration>("CameraConfiguration");
    qRegisterMetaTypeStreamOperators<QList<IModelTrackedComponent*>>("QList<IModelTrackedComponent*>");
    
    qInstallMessageHandler(myMessageOutput);

    QDir qd;
	qd.mkpath(IConfig::configLocation);
	qd.mkpath(IConfig::dataLocation);
	qd.mkpath(cfg->DirPlugins);
	qd.mkpath(cfg->DirVideos);
    qd.mkpath(cfg->DirTracks);
	qd.mkpath(cfg->DirTrials);
    qd.mkpath(cfg->DirScreenshots);
    qd.mkpath(cfg->DirTemp);

    BioTracker3App bioTracker3(&app);
    GuiContext context(&bioTracker3, cfg);
    bioTracker3.setBioTrackerContext(&context);
	bioTracker3.runBioTracker();

    app.exec();
}
