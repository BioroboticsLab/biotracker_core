#pragma once
#include "Interfaces/IModel/IModelDataExporter.h"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include <iostream>
#include <fstream>

class DataExporterCSV : public IModelDataExporter
{
	Q_OBJECT
public:
	DataExporterCSV(QObject *parent = 0);
	~DataExporterCSV();

	void open(IModelTrackedTrajectory *root) override;
	void write(int idx = -1) override;
	void writeAll() override;
	void close() override;
    void finalizeAndReInit() override;

private:
    std::string writeTrackpoint(IModelTrackedPoint *e, int trajNumber);

	IModelTrackedTrajectory *_root;
	std::ofstream _ofs;
    std::string _tmpFile;
    std::string _finalFile;
};

