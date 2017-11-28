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

    /**  
     *  Opens a file for writing the passed tracking structure
     *  @param root Pointer to the tracking structure
	 */ 
    void open(IModelTrackedTrajectory *root) override;

    /**
    *  Add a single frame index to the output file
    *  @param Index to write or -1 for latest
    */
	void write(int idx = -1) override;

    /**
    *  Re-Serialize the entire structure in a clean fashion
    */
	void writeAll() override;

    /**
    *  Close the file 
    */
	void close() override;

	void loadFile(std::string file) override {};

    /**
    *  Effectively a writeAll, close and open.
    */
    void finalizeAndReInit() override;

private:

    /**
    *  helper function to generate a part of an output line
    */
    std::string writeTrackpoint(IModelTrackedPoint *e, int trajNumber);

	IModelTrackedTrajectory *_root;
	std::ofstream _ofs;
    
    //Name of the temporary file to write to
    std::string _tmpFile;

    //Name of the final file, holding a clean representation of the structure.
    std::string _finalFile;
};

