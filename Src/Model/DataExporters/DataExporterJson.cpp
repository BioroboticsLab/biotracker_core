#include "DataExporterJson.h"
#include "util/types.h"
#include "Utility/misc.h"
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
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>


namespace DataExporterJsonUtil {
    template<typename T>
    static T preprocess_value(T &&paramValue) {
        return std::forward<T>(paramValue);
    }

    /* Writes a tracked component to a property tree for json serialization
    */
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

    /* Helper function to extract a suffix number from a string
    */
    int getId(std::string fst, std::string prefix) {
        if (fst.size() < prefix.size())
            return -1;

        std::string token = fst.substr(prefix.size());

        if (token.empty())
            return -1;

        int id = atoi(token.c_str());
        return id;
    }

    /* Helper function to set elements
    */
    void setElemProperty(boost::property_tree::ptree *tree, std::string key, IModelTrackedComponent* comp) {
        QString val = QString(tree->data().c_str());
        std::string check = val.toStdString();
        QVariant v(val);
        comp->setProperty(key.c_str(), v);
    }

    /* Recursion to write the tracked component tree into a boost property tree
    */
    void populateLevel(
        IModelTrackedComponent* comp,
        boost::property_tree::basic_ptree<std::string, std::string, std::less<std::string>> *pt,
        IModelTrackedComponentFactory* factory,
        std::deque<std::string> prefixes)
    {
        //Check the prefixes (are the children leafs or nodes?)
        std::string prefix = "";
        if (prefixes.size() > 0) {
            prefix = prefixes.front();
            prefixes.pop_front();
        }

        //Iterate children
        for (auto it : *pt) {
            //Get ID if any
            int id = getId(it.first, prefix);

            //Check if there is a subtree
            if (it.second.size() > 0) {
                //This is a valid subtree
                if (prefixes.size() > 0) {
                    IModelTrackedTrajectory *child = static_cast<IModelTrackedTrajectory*>(factory->getNewTrackedTrajectory("0"));
                    populateLevel(child, &(it.second), factory, prefixes);
                    static_cast<IModelTrackedTrajectory*>(comp)->add(child, id);
                }
                //This is a leaf node with only properties beneath
                else {
                    IModelTrackedComponent *node = static_cast<IModelTrackedComponent*>(factory->getNewTrackedElement("0"));
                    populateLevel(node, &(it.second), factory, prefixes);
                    static_cast<IModelTrackedTrajectory*>(comp)->add(node, id);
                }
            }
            //Found a property and assigning it
            else {
                setElemProperty(&(it.second), it.first, comp);
            }
        }

    }
}

DataExporterJson::DataExporterJson(QObject *parent) :
    DataExporterGeneric(parent)
{
    _root = 0;
}


DataExporterJson::~DataExporterJson()
{
    //delete _root;
}

std::string DataExporterJson::writeTrackpoint(IModelTrackedPoint *e, int trajNumber) {
    std::stringstream ss;


    return ss.str();
}

void DataExporterJson::write(int idx) {
    if (!_root) {
        qDebug() << "CORE: No output opened!";
        return;
    }

    _ofs << endl;
}

void DataExporterJson::finalizeAndReInit() {
    close(); //Not needed, but...
    writeAll();
    cleanup();
    open(_root);
}

void DataExporterJson::loadFile(std::string file) {
    boost::property_tree::ptree ptRoot;
    boost::property_tree::read_json(file, ptRoot);

    ControllerDataExporter *ctr = _parent;
    IModelTrackedComponentFactory* factory = ctr ? ctr->getComponentFactory() : nullptr;
    if (!factory) {
        return;
    }

    //ID's of entities are managed via prefix+enumeration
    std::deque<std::string> prefixes = { "Trajectory_", "Element_" };

    //Recursively reads the json
    DataExporterJsonUtil::populateLevel(
        _root,
        &ptRoot,
        factory,
        prefixes);
};

void DataExporterJson::writeAll(std::string f) {
    //Sanity
    if (!_root) {
        qDebug() << "CORE: No output opened!";
        return;
    }
    if (_oFileTmp->isOpen()) {
        _oFileTmp->close();
    }

    if (getMaxLinecount() <= 1) {
        cleanup();
        return;
    }

    std::string target = f;
    if (target.size() <= 1) {
        target = _finalFile;
    }
    if (target.substr(target.size() - 4) != ".json")
        target += ".json";

    boost::property_tree::ptree ptRoot; 
    DataExporterJsonUtil::writeComponentJson(_root, &ptRoot);
    
    //go through all trajectories
	for (int i = 0; i < _root->size(); i++) {
		IModelTrackedTrajectory *t = static_cast<IModelTrackedTrajectory *>(_root->getChild(i));
        boost::property_tree::ptree ptt;
        DataExporterJsonUtil::writeComponentJson(t, &ptt);

        ////i is the track number
        for (int idx = 0; idx < t->size(); idx++) {
            IModelTrackedComponent *e = static_cast<IModelTrackedComponent*>(t->getChild(idx));

            //If the node exists (i.e. not NULL) then it to the property tree
            if (e) {
                boost::property_tree::ptree pte;
                DataExporterJsonUtil::writeComponentJson(e, &pte);
                ptt.put_child("Element_"+std::to_string(idx), pte);
            }
        }
        //Insert the trajectory into the tree
        ptRoot.put_child("Trajectory_" + std::to_string(i), ptt);
	}

    write_json(target, ptRoot);
}

void DataExporterJson::close() {
    _oFileTmp->close();

    if ((!_root || _root->size() == 0) &&_tmpFile!="" ) {
        //Remove temporary file
        QFile file(_tmpFile.c_str());
        file.remove();
    }
}
