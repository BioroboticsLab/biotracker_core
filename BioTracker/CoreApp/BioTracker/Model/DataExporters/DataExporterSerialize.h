#pragma once

#include "DataExporterGeneric.h"

class DataExporterSerialize : public DataExporterGeneric
{
	Q_OBJECT
public:
	DataExporterSerialize(QObject *parent = 0);
	~DataExporterSerialize();

    /**
    *  Add a single frame index to the output file
    *  @param Index to write or -1 for latest
    */
	void write(int idx = -1) override;

    /**
    *  Re-Serialize the entire structure in a clean fashion
    */
	void writeAll(std::string f = "") override;

    /**
    *  Close the file 
    */
	void close() override;

	void loadFile(std::string file) override;

    /**
    *  Effectively a writeAll, close and open.
    */
    void finalizeAndReInit() override;

    QString getSuffix() { return ".dat"; };

private:

    /**
    *  helper function to generate a part of an output line
    */
    std::string writeTrackpoint(IModelTrackedPoint *e, int trajNumber);
};

