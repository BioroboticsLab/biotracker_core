#pragma once
#include "Interfaces/IModel/IModelDataExporter.h"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include "Interfaces/IModel/IModelTrackedComponentFactory.h"
#include "Controller/ControllerDataExporter.h"
#include "util/Config.h"
#include <iostream>
#include <fstream>
#include <QFile>

class DataExporterGeneric : public IModelDataExporter
{
	Q_OBJECT
public:
    DataExporterGeneric(QObject *parent = 0);

    /**  
     *  Opens a file for writing the passed tracking structure
     *  @param root Pointer to the tracking structure
	 */ 
    void open(IModelTrackedTrajectory *root) override;

    void finalize() override;

protected:

    int getMaxLinecount();

    void cleanup();

    ControllerDataExporter *_parent = nullptr;
    Config *_cfg;

    //std::ofstream::open blocks the program very often (~80% of the time)
    //std::ofstream _ofs;
    QFile* _oFileTmp;
    QTextStream _ofs;

    //Name of the temporary file to write to
    std::string _tmpFile;

    //Name of the final file, holding a clean representation of the structure.
    std::string _finalFile;
private:

};

