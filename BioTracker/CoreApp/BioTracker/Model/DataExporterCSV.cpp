#include "DataExporterCSV.h"
#include "util/types.h"
#include "util/misc.h"
#include <qdebug.h>
#include <qfile.h>

DataExporterCSV::DataExporterCSV(QObject *parent) :
    IModelDataExporter(parent)
{
    _root = 0;
}


DataExporterCSV::~DataExporterCSV()
{
    //delete _root;
}

void DataExporterCSV::open(IModelTrackedTrajectory *root) {
    _root = root;
    //We need to use "time(source_ms)" instead of the previous "time(source, ms)".
    //Apparently conventional tools like Excel and OOCalc got some issues parsing this correctly (naivly search for commas...)
    std::string baseName = getTimeAndDate(CFG_DIR_TRACKS, "");
    _tmpFile = baseName + ".tmp.csv";
    _finalFile = baseName + ".csv";
    _ofs.open(_tmpFile, std::ofstream::out);
    _ofs
        << "frame No."
        << ",time(source_ms)"
        << ",time(tracking_ms)"
        << ",ID"
        << ",x"
        << ",y"
        << ",deg"
        << ",rad"
        << std::endl;

}

std::string DataExporterCSV::writeTrackpoint(IModelTrackedPoint *e, int trajNumber) {
    std::stringstream ss;
    ss << (trajNumber > 0 ? "," : "") << e->getId()
        << "," + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(e->getTime().time_since_epoch()).count())
        << "," + e->getCoordinateUnit().toStdString()
        << (e->hasX() ? "," + std::to_string(e->getX()) : "")
        << (e->hasY() ? "," + std::to_string(e->getY()) : "")
        << (e->hasW() ? "," + std::to_string(e->getW()) : "")
        << (e->hasH() ? "," + std::to_string(e->getH()) : "")
        << (e->hasDeg() ? "," + std::to_string(e->getDeg()) : "")
        << (e->hasRad() ? "," + std::to_string(e->getRad()) : "");
    std::string s = ss.str();

    return s;
}

void DataExporterCSV::write(int idx) {
    if (!_root) {
        qDebug() << "No output opened!";
        return;
    }

    //Write single trajectory
    int trajNumber = 0;
    for (int i = 0; i < _root->size(); i++) {
        //TODO there is some duplicated code here
        _ofs << std::to_string(idx)
            << "," + std::to_string((long long)((((double)idx) / _fps) * 1000));

        IModelTrackedTrajectory *t = dynamic_cast<IModelTrackedTrajectory *>(_root->getChild(i));
        if (t) {
            IModelTrackedPoint *e;
            if (idx == -1)
                e = dynamic_cast<IModelTrackedPoint*>(t->getLastChild());
            else
                e = dynamic_cast<IModelTrackedPoint*>(t->getChild(idx));
            if (e && e->getValid()) {
                _ofs << writeTrackpoint(e, trajNumber);
            }
            trajNumber++;
        }
    }
    _ofs << std::endl;
}

void DataExporterCSV::finalizeAndReInit() {
    close(); //Not needed, but...
    writeAll();
    open(_root);
}

void DataExporterCSV::writeAll() {
    //Sanity
    if (!_root) {
        qDebug() << "No output opened!";
        return;
    }
    if (_ofs.is_open()) {
        _ofs.close();
    }

    //Create final file
    std::ofstream o;
    o.open(_finalFile, std::ofstream::out);

    //Find max length of all tracks
    int max = 0;
    for (int i = 0; i < _root->size(); i++) {
        IModelTrackedTrajectory *t = dynamic_cast<IModelTrackedTrajectory *>(_root->getChild(i));
        if (t) max = std::max(t->size(), max);
    }

    //Write out everything to a new file
    int trajNumber = 0;

    //idx is the frame number
    for (int idx = 0; idx < max; idx++) {
        //i is the track number
        for (int i = 0; i < _root->size(); i++) {

            o << std::to_string(idx)
                << "," + std::to_string((((float)idx) / _fps) * 1000);

            IModelTrackedTrajectory *t = dynamic_cast<IModelTrackedTrajectory *>(_root->getChild(i));
            if (t) {
                IModelTrackedPoint *e = dynamic_cast<IModelTrackedPoint*>(t->getChild(idx));
                if (e) {
                    o << writeTrackpoint(e, trajNumber);
                }
                trajNumber++;
            }
        }
        o << std::endl;
    }
    o.close();

    //Erase all tracking data from the tracking structure!
    _root->clear();

    //Remove temporary file
    QFile file(_tmpFile.c_str());
    file.remove();
}

void DataExporterCSV::close() {
    _ofs.close();

    if (!_root || _root->size() == 0) {
        //Remove temporary file
        QFile file(_tmpFile.c_str());
        file.remove();
    }
}
