#include "ControllerDataExporter.h"
#include "Model/DataExporterCSV.h"
#include "Model/DataExporterSerialize.h"
#include "Model/DataExporterJson.h"
#include "settings/Settings.h"
#include "util/types.h"


ControllerDataExporter::ControllerDataExporter(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
	IController(parent, context, ctr)
{
}

ControllerDataExporter::~ControllerDataExporter()
{
	if (m_Model)
		qobject_cast<IModelDataExporter*>(m_Model)->finalize();
	
}

void ControllerDataExporter::connectControllerToController() {
	//IController* ctrM = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
	//QPointer< MainWindow > mainWin = dynamic_cast<MainWindow*>(ctrM->getView());
	//mainWin->addVideoControllWidget(m_View);
}

void ControllerDataExporter::createModel() {
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
		m_Model = 0;
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

void ControllerDataExporter::createView() {

	m_View = 0;
}

void ControllerDataExporter::setDataStructure(IModel* exp) {
	qobject_cast<IModelDataExporter*>(m_Model)->open(static_cast<IModelTrackedTrajectory*>(exp));
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
        dynamic_cast<IModelDataExporter*>(getModel())->finalizeAndReInit();
    }
}

void ControllerDataExporter::receiveReset() {
    if (getModel()) {
        dynamic_cast<IModelDataExporter*>(getModel())->finalizeAndReInit();
        dynamic_cast<IModelDataExporter*>(getModel())->close();
    }

    createModel();
}

void ControllerDataExporter::connectModelToController() {
	IController* ctrM = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
	MediaPlayer* mplay = dynamic_cast<MediaPlayer*>(ctrM->getModel());

	QObject::connect(mplay, &MediaPlayer::fwdPlayerParameters, this, &ControllerDataExporter::rcvPlayerParameters);
}

void ControllerDataExporter::rcvPlayerParameters(playerParameters* parameters) {
	qobject_cast<IModelDataExporter*>(m_Model)->setFps(parameters->m_fpsSourceVideo);
	qobject_cast<IModelDataExporter*>(m_Model)->setTitle(parameters->m_CurrentTitle);
}