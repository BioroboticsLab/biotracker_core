#pragma once

#include "IModel.h"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include <string.h>

class IModelDataExporter :public IModel
{
	Q_OBJECT

public:
	IModelDataExporter(QObject *parent = 0);
	~IModelDataExporter();

	virtual void open(IModelTrackedTrajectory *root, IModelTrackedComponent *hint = 0, bool append = false, std::string file = "") = 0;
	virtual void writeLatest() = 0;
	virtual void writeAll() = 0;
	virtual void close() = 0;

public:
	IModelTrackedTrajectory *_root;
};

