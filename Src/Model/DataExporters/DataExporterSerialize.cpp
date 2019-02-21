#include "DataExporterSerialize.h"
#include "util/types.h"
#include "Utility/misc.h"
#include <qdebug.h>
#include <qfile.h>

#include <qdatastream.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "Controller/ControllerDataExporter.h"

DataExporterSerialize::DataExporterSerialize(QObject *parent) :
    DataExporterGeneric(parent)
{
    _root = 0;
}


DataExporterSerialize::~DataExporterSerialize()
{
}

std::string DataExporterSerialize::writeTrackpoint(IModelTrackedPoint *e, int trajNumber) {
    std::stringstream ss;


    return ss.str();
}

void DataExporterSerialize::write(int idx) {
    if (!_root) {
        qDebug() << "CORE: No output opened!";
        return;
    }

    _ofs << endl;
}

void DataExporterSerialize::finalizeAndReInit() {
    close(); //Not needed, but...
    writeAll();
    cleanup();
    open(_root);
}

void DataExporterSerialize::loadFile(std::string file){

	ControllerDataExporter *ctr = _parent;
    IModelTrackedComponentFactory* factory = ctr ? ctr->getComponentFactory() : nullptr;
	if (!factory) {
		return;
	}

	QFile f(file.c_str());
	f.open(QIODevice::ReadOnly);
	QDataStream in(&f);

    IModelTrackedTrajectory *root = _root;
    in >> *root;
	int children = -1;
	in >> children;

	std::vector<int> childids;
	for (int i = 0; i < children; i++) {
		IModelTrackedTrajectory *child = static_cast<IModelTrackedTrajectory*>(factory->getNewTrackedTrajectory("0"));
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
			IModelTrackedComponent *e = factory->getNewTrackedElement("0");
            int cid = 0;
            in >> cid;
			in >> *e;
			curTraj->add(e, cid);
		}
	}
};

void DataExporterSerialize::writeAll(std::string f) {
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
    if (target.substr(target.size() - 4) != ".dat")
        target += ".dat";

    //Create final file
	QFile file(target.c_str());
	file.open(QIODevice::WriteOnly);
	QDataStream out(&file);

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

        int cnt = 0;
        for (int idx = 0; idx < t->size(); idx++) {
            if (t) {
                IModelTrackedComponent *e = dynamic_cast<IModelTrackedComponent*>(t->getChild(idx));
                if (e) {
                    cnt++;
                }
            }
        }
        out << cnt;

		//idx is the frame number
		for (int idx = 0; idx < t->size(); idx++) {
            if (t) {
				IModelTrackedComponent *e = dynamic_cast<IModelTrackedComponent*>(t->getChild(idx));
                if (e) {
                    out << idx;
                    out << *e;
                }
            }
        }
    }
}

void DataExporterSerialize::close() {
    _oFileTmp->close();

    if ((!_root || _root->size() == 0) && _tmpFile!="" ) {
        //Remove temporary file
        QFile file(_tmpFile.c_str());
        file.remove();
    }
}
