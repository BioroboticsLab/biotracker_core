#pragma once

#include "IModel.h"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include <string.h>
#include <qfileinfo.h>

class IModelDataExporter :public IModel
{
	Q_OBJECT

public:
	IModelDataExporter(QObject *parent = 0);
	~IModelDataExporter();

	virtual void open(IModelTrackedTrajectory *root) = 0;
	virtual void write(int idx) = 0;
	virtual void writeAll() = 0;
    virtual void close() = 0;
    virtual void finalizeAndReInit() = 0;
	void setFps(float fps) { _fps = fps; };
	void setTitle(std::string title) { _title = title; };
	void finalize() {
		close();
		writeAll();
	};

	virtual void loadFile(std::string file) = 0;

public:
	IModelTrackedTrajectory *_root;
	float _fps;
	std::string _title;

signals:
    void fileWritten(QFileInfo file);
};

