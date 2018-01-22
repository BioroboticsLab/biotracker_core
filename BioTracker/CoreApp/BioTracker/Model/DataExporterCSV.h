#pragma once
#include "Interfaces/IModel/IModelDataExporter.h"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include "Interfaces/IModel/IModelTrackedComponentFactory.h"
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

	void loadFile(std::string file) override;

    /**
    *  Effectively a writeAll, close and open.
    */
    void finalizeAndReInit() override;

private:

    QObject *_parent = nullptr;

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

