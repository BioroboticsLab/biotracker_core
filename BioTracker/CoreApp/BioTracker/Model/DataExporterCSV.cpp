#include "DataExporterCSV.h"
#include "util/types.h"
#include "util/misc.h"
#include <qdebug.h>

DataExporterCSV::DataExporterCSV(QObject *parent) :
	IModelDataExporter(parent)
{
	_root = 0;
}


DataExporterCSV::~DataExporterCSV()
{
	//delete _root;
}

void DataExporterCSV::open(IModelTrackedTrajectory *root, IModelTrackedComponent *hint, bool append, std::string file) {
	_root = root;
	IModelTrackedPoint *t = dynamic_cast<IModelTrackedPoint*>(hint);
	if (!t)
	{
		for (int i = 0; i < _root->size(); i++) {
			t = dynamic_cast<IModelTrackedPoint *>(_root->getChild(i));
			if (t) {
				break;
			}
		}
	}

	_ofs.open(getTimeAndDate(CFG_DIR_TRACKS, ".csv"), std::ofstream::out);
	if(t)
	_ofs << "ID"
		<< ",frame No."
		<< ",time(source, ms)"
		<< (t->hasTime() ? ",time(tracking, ms)" : "")
		<< (t->hasX() ? ",x" : "")
		<< (t->hasY() ? ",y" : "")
		<< (t->hasZ() ? ",z" : "")
		//<< (t->hasW() ? ",w" : "")
		//<< (t->hasH() ? ",h" : "")
		<< (t->hasDeg() ? ",deg" : "")
		<< (t->hasRad() ? ",rad" : "")
		<< std::endl;

}

void DataExporterCSV::write(int idx) {
	if (!_root) {
		qDebug() << "No output opened!";
		return;
	}

	int trajNumber = 0;
	for (int i = 0; i < _root->size(); i++) {
		IModelTrackedTrajectory *t = dynamic_cast<IModelTrackedTrajectory *>(_root->getChild(i));
		if (t) {
			IModelTrackedPoint *e;
			if (idx == -1)
				e = dynamic_cast<IModelTrackedPoint*>(t->getLastChild());
			else
				e = dynamic_cast<IModelTrackedPoint*>(t->getChild(idx));
			if (e->getValid())
				_ofs << (trajNumber > 0 ? "," : "") << e->getId()
				<< "," + std::to_string(idx)
				<< "," + std::to_string((((float)idx) / _fps) *1000)
				<< (e->hasTime() ? "," + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(e->getTime().time_since_epoch()).count()) : "")
					<< (e->hasX() ? "," + std::to_string(e->getX()) : "")
					<< (e->hasY() ? "," + std::to_string(e->getY()) : "")
					<< (e->hasZ() ? "," + std::to_string(e->getZ()) : "")
					//<< (e->hasW() ? "," + std::to_string(e->getW()) : "")
					//<< (e->hasH() ? "," + std::to_string(e->getH()) : "")
					<< (e->hasDeg() ? "," + std::to_string(e->getDeg()) : "")
					<< (e->hasRad() ? "," + std::to_string(e->getRad()) : "");
			trajNumber++;
		}
	}
	_ofs << std::endl;
}

void DataExporterCSV::writeAll() {

}

void DataExporterCSV::close() {

}
