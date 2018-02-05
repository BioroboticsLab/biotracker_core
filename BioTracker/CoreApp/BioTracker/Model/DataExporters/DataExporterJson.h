#pragma once

#include "DataExporterGeneric.h"

class DataExporterJson : public DataExporterGeneric
{
	Q_OBJECT
public:
    DataExporterJson(QObject *parent = 0);
	~DataExporterJson();

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

	void loadFile(std::string file) override;

    /**
    *  Effectively a writeAll, close and open.
    */
    void finalizeAndReInit() override;

    QString getSuffix() { return ".json"; };

private:

    /**
    *  helper function to generate a part of an output line
    */
    std::string writeTrackpoint(IModelTrackedPoint *e, int trajNumber);
};

