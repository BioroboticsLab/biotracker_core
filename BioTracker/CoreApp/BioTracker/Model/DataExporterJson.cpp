#include "DataExporterJson.h"
#include "util/types.h"
#include "util/misc.h"
#include <qdebug.h>
#include <qfile.h>

#include "Controller/ControllerDataExporter.h"


#include <qfile.h>
#include <qdatastream.h>
#include <qdebug.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <type_traits>

DataExporterJson::DataExporterJson(QObject *parent) :
    IModelDataExporter(parent)
{
	_parent = parent;
    _root = 0;
}


DataExporterJson::~DataExporterJson()
{
    //delete _root;
}

void DataExporterJson::open(IModelTrackedTrajectory *root) {
    _root = root;
    //We need to use "time(source_ms)" instead of the previous "time(source, ms)".
    //Apparently conventional tools like Excel and OOCalc got some issues parsing this correctly (naivly search for commas...)
    std::string baseName = getTimeAndDate(CFG_DIR_TRACKS, "");
    _tmpFile = baseName + ".tmp.json";
    _finalFile = baseName + ".json";
    _ofs.open(_tmpFile, std::ofstream::out);

}

std::string DataExporterJson::writeTrackpoint(IModelTrackedPoint *e, int trajNumber) {
    std::stringstream ss;


    return ss.str();
}

void DataExporterJson::write(int idx) {
    if (!_root) {
        qDebug() << "No output opened!";
        return;
    }

    _ofs << std::endl;
}

void DataExporterJson::finalizeAndReInit() {
    close(); //Not needed, but...
    writeAll();
    open(_root);
}

void DataExporterJson::loadFile(std::string file){

};

template<typename T>
static T preprocess_value(T &&paramValue) {
    return std::forward<T>(paramValue);
}

void writeComponentJson(IModelTrackedComponent* comp, boost::property_tree::ptree *pt) {

    for (int i = 0; i<comp->metaObject()->propertyCount(); ++i)
    {
        if (comp->metaObject()->property(i).isStored(comp))
        {
            std::string str = comp->metaObject()->property(i).name();
            QVariant v = comp->metaObject()->property(i).read(comp);
            pt->put(str, v.toString().toStdString());
        }
    }
}

void DataExporterJson::writeAll() {
    //Sanity
    if (!_root) {
        qDebug() << "No output opened!";
        return;
    }
    if (_ofs.is_open()) {
        _ofs.close();
    }

    boost::property_tree::ptree ptRoot;
    writeComponentJson(_root, &ptRoot);
    
	for (int i = 0; i < _root->size(); i++) {
		IModelTrackedTrajectory *t = static_cast<IModelTrackedTrajectory *>(_root->getChild(i));
        boost::property_tree::ptree ptt;
        writeComponentJson(t, &ptt);

        ////i is the track number
        for (int idx = 0; idx < t->size(); idx++) {
            IModelTrackedComponent *e = static_cast<IModelTrackedComponent*>(t->getChild(idx));
            if (e) {
                boost::property_tree::ptree pte;
                writeComponentJson(e, &pte);
                ptt.put_child("Element_"+std::to_string(idx), pte);
            }
        }
        ptRoot.put_child("Trajectory_" + std::to_string(i), ptt);
	}

    write_json(_finalFile, ptRoot);
    //Erase all tracking data from the tracking structure!
    _root->clear();

    //Remove temporary file
    QFile ft(_tmpFile.c_str());
    ft.remove();
}

void DataExporterJson::close() {
    _ofs.close();

    if (!_root || _root->size() == 0) {
        //Remove temporary file
        QFile file(_tmpFile.c_str());
        file.remove();
    }
}
