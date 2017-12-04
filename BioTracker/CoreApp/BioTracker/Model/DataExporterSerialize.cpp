#include "DataExporterSerialize.h"
#include "util/types.h"
#include "util/misc.h"
#include <qdebug.h>
#include <qfile.h>

#include "Controller/ControllerDataExporter.h"

DataExporterSerialize::DataExporterSerialize(QObject *parent) :
    IModelDataExporter(parent)
{
	_parent = parent;
    _root = 0;
}


DataExporterSerialize::~DataExporterSerialize()
{
    //delete _root;
}

void DataExporterSerialize::open(IModelTrackedTrajectory *root) {
    _root = root;
    //We need to use "time(source_ms)" instead of the previous "time(source, ms)".
    //Apparently conventional tools like Excel and OOCalc got some issues parsing this correctly (naivly search for commas...)
    std::string baseName = getTimeAndDate(CFG_DIR_TRACKS, "");
    _tmpFile = baseName + ".tmp.dat";
    _finalFile = baseName + ".dat";
    _ofs.open(_tmpFile, std::ofstream::out);

}

std::string DataExporterSerialize::writeTrackpoint(IModelTrackedPoint *e, int trajNumber) {
    std::stringstream ss;


    return ss.str();
}

void DataExporterSerialize::write(int idx) {
    if (!_root) {
        qDebug() << "No output opened!";
        return;
    }

    _ofs << std::endl;
}

void DataExporterSerialize::finalizeAndReInit() {
    close(); //Not needed, but...
    writeAll();
    open(_root);
}



#include <qfile.h>
#include <qdatastream.h>
#include <qdebug.h>
#include <iostream>
#include <fstream>
#include <vector>
void DataExporterSerialize::loadFile(std::string file){

	ControllerDataExporter *ctr = dynamic_cast<ControllerDataExporter*>(_parent);
	IModelTrackedComponentFactory* factory = ctr ? ctr->getComponentFactory() : nullptr;
	if (!factory) {
		return;
	}

	QFile f(file.c_str());
	f.open(QIODevice::ReadOnly);
	QDataStream in(&f);


	IModelTrackedTrajectory *root = _root;// static_cast<IModelTrackedTrajectory*>(factory->getNewTrackedTrajectory());
	in >> *root;
	int children = -1;
	in >> children;

	std::vector<int> childids;
	for (int i = 0; i < children; i++) {
		IModelTrackedTrajectory *child = static_cast<IModelTrackedTrajectory*>(factory->getNewTrackedTrajectory());
		in >> *child;
		childids.push_back(child->getId());
		root->add(child, child->getId());
	}

	//i is the track number
	for (int i = 0; i < childids.size(); i++) {
		IModelTrackedTrajectory* curTraj = static_cast<IModelTrackedTrajectory*>(root->getChild(childids[i]));
		int trajSize = -1;
		in >> trajSize;
		//idx is the frame number
		for (int idx = 0; idx < trajSize; idx++) {
			IModelTrackedComponent *e = factory->getNewTrackedElement();
			in >> *e;
			curTraj->add(e,idx);
		}
	}

	//int idx = -1, i = -1;
	//in >> idx;
	//in >> i;
	//IModelTrackedComponent *e = factory->getNewTrackedElement();
	//in >> *e;
};

void DataExporterSerialize::writeAll() {
    //Sanity
    if (!_root) {
        qDebug() << "No output opened!";
        return;
    }
    if (_ofs.is_open()) {
        _ofs.close();
    }

    //Create final file
	QFile file(_finalFile.c_str());
	file.open(QIODevice::WriteOnly);

	QDataStream out(&file); 

	//IModelTrackedTrajectory *t = dynamic_cast<IModelTrackedTrajectory *>(_root->getChild(0));
	//IModelTrackedComponent *e = dynamic_cast<IModelTrackedComponent*>(t->getChild(5));
	//out << 42;
	//out << 1337;
	//out << *e;
	//return;

	//serialize tree nodes (!= leafs)
	out << *_root;
	out << _root->size();
	for (int i = 0; i < _root->size(); i++) {
		IModelTrackedTrajectory *t = dynamic_cast<IModelTrackedTrajectory *>(_root->getChild(i));
		out << *t;
	}


	//i is the track number
	for (int i = 0; i < _root->size(); i++) {
		IModelTrackedTrajectory *t = dynamic_cast<IModelTrackedTrajectory *>(_root->getChild(i));
		out << t->size();
		//idx is the frame number
		for (int idx = 0; idx < t->size(); idx++) {
            if (t) {
				IModelTrackedComponent *e = dynamic_cast<IModelTrackedComponent*>(t->getChild(idx));
				out << *e;
            }
        }
    }

    //Erase all tracking data from the tracking structure!
    _root->clear();

    //Remove temporary file
    QFile ft(_tmpFile.c_str());
    ft.remove();
}

void DataExporterSerialize::close() {
    _ofs.close();
}
