#pragma once
#include "DataExporterGeneric.h"

class DataExporterCSV : public DataExporterGeneric
{
	Q_OBJECT
public:
	DataExporterCSV(QObject *parent = 0);
	~DataExporterCSV();

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

    QString getSuffix() { return ".csv"; };

private:

    /**
    *  helper function to generate a part of an output line
    */
    std::string writeTrackpoint(IModelTrackedPoint *e, int trajNumber);

    std::string _separator;


    /* finding the number of occurrences of a string in another strin
     *
     * https://codereview.stackexchange.com/questions/29611/finding-the-number-of-occurrences-of-a-string-in-another-string
     */
    int key_search(const std::string& s, const std::string& key);

    /* Get's the names of all stored properties.
     * 
     * Useful for corresponding them with CSV headers. order relevant.
    * returns Individual1
     */
    std::vector<std::string> getHeaderElements(IModelTrackedComponent *comp);
   
    /* Get's the header as one connected string
    *
    * Similar getHeaderElements, but for all components and having all elements of the return vector concatenated
    * returns GlobalHeaders,Individual1,Indiviual2,...
    */
    std::string getHeader(IModelTrackedComponent *comp, int cnt);

    /* Sets sets a property to a components.
    */
    void setProperty(IModelTrackedComponent* comp, std::string key, std::string val);

    /* Writes a tracked component to string
    */
    std::string writeComponentCSV(IModelTrackedComponent* comp, int tid);


    void addChildOfChild(IModelTrackedTrajectory *root, IModelTrackedComponent* child, IModelTrackedComponentFactory* factory, int idx);
};

