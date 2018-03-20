#include "ControllerDataExporter.h"
#include "Controller/ControllerCommands.h"
#include "Controller/ControllerTrackedComponentCore.h"
#include "Model/DataExporters/DataExporterCSV.h"
#include "Model/DataExporters/DataExporterSerialize.h"
#include "Model/DataExporters/DataExporterJson.h"
#include "settings/Settings.h"
#include "util/types.h"
#include <qmessagebox.h>


ControllerDataExporter::ControllerDataExporter(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
	IController(parent, context, ctr)
{
}

ControllerDataExporter::~ControllerDataExporter()
{
}

void ControllerDataExporter::cleanup() {
    if (m_Model)
   	    qobject_cast<IModelDataExporter*>(m_Model)->finalize();
}

void ControllerDataExporter::connectControllerToController() {
    //connect to controller commands
	IController* ictrcmd = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::COMMANDS);
	ControllerCommands *ctrcmd = static_cast<ControllerCommands*>(ictrcmd);

    QObject::connect(this, &ControllerDataExporter::emitResetUndoStack, ctrcmd, &ControllerCommands::receiveClear, Qt::DirectConnection);

    //connect to view controller
    IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE
    );
	ControllerTrackedComponentCore *tccController = dynamic_cast<ControllerTrackedComponentCore*>(ctr);

	QObject::connect(this, &ControllerDataExporter::emitViewUpdate, tccController, &ControllerTrackedComponentCore::receiveUpdateView, Qt::DirectConnection);
}

void ControllerDataExporter::createModel() {
    m_Model = nullptr;
}

SourceVideoMetadata ControllerDataExporter::getSourceMetadata() {
	IController* ctrM = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
	MediaPlayer* mplay = dynamic_cast<MediaPlayer*>(ctrM->getModel());
	SourceVideoMetadata d;
	d.name = mplay->getCurrentFileName().toStdString();
	d.fps = std::to_string(mplay->getFpsOfSourceFile());
	d.fps = d.fps.erase(d.fps.find_last_not_of('0') + 1, std::string::npos);
	return d;
}

void ControllerDataExporter::loadFile(std::string file) {
    if (_factory) {
        qobject_cast<IModelDataExporter*>(m_Model)->loadFile(file);
    }
    else {
        std::cout << "Can not load tracks for this plugin as it does not provide a factory." << std::endl;
    }
}

void ControllerDataExporter::saveFile(std::string file) {
    if (_factory) {
        qobject_cast<IModelDataExporter*>(m_Model)->writeAll(file);
    }
    else {
        std::cout << "Can not save tracks for this plugin as it does not provide a factory." << std::endl;
    }
}

void ControllerDataExporter::createView() {

	m_View = 0;
}

void ControllerDataExporter::setDataStructure(IModel* exp) {
    if (getModel())
        delete getModel();

    //Grab the codec from config file
    BioTracker::Core::Settings *set = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
    std::string exporter = exporterList[set->getValueOrDefault<int>(CFG_EXPORTER, 0)];
    if (exporter == "CSV")
        m_Model = new DataExporterCSV(this);
    else if (exporter == "Serialize")
        m_Model = new DataExporterSerialize(this);
    else if (exporter == "Json")
        m_Model = new DataExporterJson(this);
    else
        m_Model = nullptr;

    qobject_cast<IModelDataExporter*>(m_Model)->open(static_cast<IModelTrackedTrajectory*>(exp));

    IModelDataExporter* model;
    if ((model = dynamic_cast<IModelDataExporter*>(getModel())) != nullptr) {
        QObject::connect(model, &IModelDataExporter::fileWritten, this, &ControllerDataExporter::receiveFileWritten);
    }

}

void ControllerDataExporter::setComponentFactory(IModelTrackedComponentFactory* exp) {
	_factory = exp;
}

void ControllerDataExporter::receiveTrackingDone(uint frame) {
    if (getModel()) {
        dynamic_cast<IModelDataExporter*>(getModel())->write(frame);
    }
}

void ControllerDataExporter::receiveFinalizeExperiment() {
    if (getModel()) {
        emitResetUndoStack();
        dynamic_cast<IModelDataExporter*>(getModel())->finalizeAndReInit();
        emitViewUpdate();
    }
}

void ControllerDataExporter::receiveReset() {
    if (getModel()) {
        emitResetUndoStack();
        dynamic_cast<IModelDataExporter*>(getModel())->finalizeAndReInit();
        dynamic_cast<IModelDataExporter*>(getModel())->close();
        emitViewUpdate();
    }

    createModel();
}

void ControllerDataExporter::connectModelToController() {
	IController* ctrM = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
	MediaPlayer* mplay = dynamic_cast<MediaPlayer*>(ctrM->getModel());

    QObject::connect(mplay, &MediaPlayer::fwdPlayerParameters, this, &ControllerDataExporter::rcvPlayerParameters);
}

void ControllerDataExporter::rcvPlayerParameters(playerParameters* parameters) {
    if (qobject_cast<IModelDataExporter*>(m_Model) != nullptr) {
        qobject_cast<IModelDataExporter*>(m_Model)->setFps(parameters->m_fpsSourceVideo);
        qobject_cast<IModelDataExporter*>(m_Model)->setTitle(parameters->m_CurrentTitle);
    }
}

int ControllerDataExporter::getTrialNumber() {
    //Get all existing files and parse highest export number
    QString basePath = _trialStarted ? CFG_DIR_TRIALS : CFG_DIR_TRACKS;
    QStringList allFiles = QDir(basePath).entryList(QDir::NoDotAndDotDot | QDir::Files);

    int maxVal = 0;
    foreach(QString s, allFiles) {
        int val;
        if (sscanf(s.toStdString().c_str(), "Export_%d_*", &val) != EOF) {
            maxVal = std::max(maxVal, val);
        }
    }
    return maxVal;
}

QString ControllerDataExporter::generateBasename(bool temporaryFile) {

	QString resultPath = (_trialStarted ? CFG_DIR_TRIALS : CFG_DIR_TRACKS);

    QString path = (temporaryFile ? CFG_DIR_TEMP : resultPath);
    int maxVal = getTrialNumber();
    std::string current = "Export_"+std::to_string(maxVal+1)+"_";

    return QString(getTimeAndDate(path.toStdString() + current, "").c_str());
}

void ControllerDataExporter::receiveFileWritten(QFileInfo fname) {

    QString str = "Exported file:\n";
    str += fname.absoluteFilePath();

    int ret = QMessageBox::information(nullptr, QString("Trajectory Exporting"),
        str,
        QMessageBox::Ok);
}

void ControllerDataExporter::receiveTrialStarted(bool started)
{
	_trialStarted = started;
}

