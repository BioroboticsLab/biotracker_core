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

template<typename T>
static T preprocess_value(T &&paramValue) {
    return std::forward<T>(paramValue);
}

void writeComponentJson(IModelTrackedComponent* comp, boost::property_tree::ptree *pt) {

    for (int i = 0; i<comp->metaObject()->propertyCount(); ++i)
    {
        if (comp->metaObject()->property(i).isStored(comp))
        {
            if (!comp->metaObject()->property(i).isStored()) {
                continue;
            }
            std::string str = comp->metaObject()->property(i).name();
            QVariant v = comp->metaObject()->property(i).read(comp);
            pt->put(str, v.toString().toStdString());
        }
    }
}

int getId(std::string fst, std::string prefix) {
    if (fst.size() < prefix.size())
        return -1;

    std::string token = fst.substr(prefix.size());

    if (token.empty())
        return -1;

    int id = atoi(token.c_str());
    return id;
}

void setElemProperty(boost::property_tree::ptree *tree, std::string key, IModelTrackedComponent* comp) {
    QString val = QString(tree->data().c_str());
    std::string check = val.toStdString();
    QVariant v(val);
    comp->setProperty(key.c_str(), v);
}

void populateLevel(
    IModelTrackedComponent* comp,
    boost::property_tree::basic_ptree<std::string, std::string, std::less<std::string>> *pt,
    IModelTrackedComponentFactory* factory,
    std::deque<std::string> prefixes) 
{
    std::string prefix = "";
    if (prefixes.size() > 0) {
        prefix = prefixes.front();
        prefixes.pop_front();
    }
    //qDebug() << prefix.c_str();

    for (auto it : *pt) {
        int id = getId(it.first, prefix);
        //qDebug() << id;

        if (it.second.size() > 0) {
            if (prefixes.size() > 0) {
                IModelTrackedTrajectory *child = static_cast<IModelTrackedTrajectory*>(factory->getNewTrackedTrajectory());
                //qDebug() << "TR";
                populateLevel(child, &(it.second), factory, prefixes);
                static_cast<IModelTrackedTrajectory*>(comp)->add(child, id);
            }
            else {
                IModelTrackedComponent *node = static_cast<IModelTrackedComponent*>(factory->getNewTrackedElement());
                //qDebug() << "ND";
                populateLevel(node, &(it.second), factory, prefixes);
                static_cast<IModelTrackedTrajectory*>(comp)->add(node, id);
            }
        }
        else {
            setElemProperty(&(it.second), it.first, comp);
            //qDebug() << "PR " << it.first.c_str();
        }
    }

}

void DataExporterJson::loadFile(std::string file) {
    boost::property_tree::ptree ptRoot;
    boost::property_tree::read_json(file, ptRoot);

    ControllerDataExporter *ctr = dynamic_cast<ControllerDataExporter*>(_parent);
    IModelTrackedComponentFactory* factory = ctr ? ctr->getComponentFactory() : nullptr;
    if (!factory) {
        return;
    }
    //IModelTrackedTrajectory *root = static_cast<IModelTrackedTrajectory*>(factory->getNewTrackedTrajectory());
    //_root = root;
    std::deque<std::string> prefixes = { "Trajectory_", "Element_" };
    /*IModelTrackedTrajectory *child = static_cast<IModelTrackedTrajectory*>(factory->getNewTrackedTrajectory());
    for (int i = 1; i < 100; i++) {
        IModelTrackedComponent *node = static_cast<IModelTrackedComponent*>(factory->getNewTrackedElement());
        node->setProperty("x", QVariant(24));
        node->setProperty("y", QVariant(33));
        child->add(node, 100);
    }
    _root->add(child, 100);
    return;*/

    populateLevel(
        _root,
        &ptRoot,
        factory,
        prefixes);
};

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
