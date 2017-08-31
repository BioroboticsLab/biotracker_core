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

	void open(IModelTrackedTrajectory *root, IModelTrackedComponent *hint = 0, bool append = false, std::string file = "") override;
	void write(int idx = -1) override;
	void writeAll() override;
	void close() override;

private:
	IModelTrackedTrajectory *_root;
	std::ofstream _ofs;
};

