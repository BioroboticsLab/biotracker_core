#pragma once

#include "Interfaces/IController/IController.h"
#include "Interfaces/IBioTrackerPlugin.h"
#include "Interfaces/IModel/IModelTrackedComponentFactory.h"
#include "QPointer"
#include "QThread"
#include "Model/MediaPlayer.h"

//POD class to bundle some infos
struct SourceVideoMetadata {
	std::string name;
	std::string fps;
};

class ControllerDataExporter : public IController {
	Q_OBJECT
public:
	ControllerDataExporter(QObject* parent = 0, IBioTrackerContext* context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);
	~ControllerDataExporter();

	// IController interface
public:
	void connectControllerToController() override;
	void setDataStructure(IModel* exp);
	void setComponentFactory(IModelTrackedComponentFactory* exp);
	IModelTrackedComponentFactory* getComponentFactory() { return _factory; };
	SourceVideoMetadata getSourceMetadata();
    QString generateBasename(bool temporaryFile);

	void loadFile(std::string file);

	public Q_SLOTS:
    void receiveReset();
	void receiveTrackingDone(uint frame);
    void receiveFinalizeExperiment();
    void receiveFileWritten(QFileInfo fname);

protected:
	void createModel() override;
	void createView() override;
	void connectModelToController() override;

private Q_SLOTS:
	void rcvPlayerParameters(playerParameters* parameters);

private:
	IModelTrackedComponentFactory* _factory;
};

