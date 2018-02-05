#pragma once
#include "Interfaces/IModel/IModelDataExporter.h"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include "Interfaces/IModel/IModelTrackedComponentFactory.h"
#include <iostream>
#include <fstream>

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

protected:

    int getMaxLinecount();

    void cleanup();

    QObject *_parent = nullptr;

    std::ofstream _ofs;

    //Name of the temporary file to write to
    std::string _tmpFile;

    //Name of the final file, holding a clean representation of the structure.
    std::string _finalFile;
private:

};

