#include "DataExporterCSV.h"



DataExporterCSV::DataExporterCSV(QObject *parent) :
	IModelDataExporter(parent)
{
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

	_ofs.open("TrackingOutput.txt", std::ofstream::out);
	if(t)
	_ofs << "ID"
		<< (t->hasTime() ? ",time(ms)" : "")
		<< (t->hasX() ? ",x" : "")
		<< (t->hasY() ? ",y" : "")
		<< (t->hasZ() ? ",z" : "")
		<< (t->hasW() ? ",w" : "")
		<< (t->hasH() ? ",h" : "")
		<< (t->hasDeg() ? ",deg" : "")
		<< (t->hasRad() ? ",rad" : "")
		<< std::endl;

}

void DataExporterCSV::writeLatest() {
	int trajNumber = 0;
	for (int i = 0; i < _root->size(); i++) {
		IModelTrackedTrajectory *t = dynamic_cast<IModelTrackedTrajectory *>(_root->getChild(i));
		if (t) {
			IModelTrackedPoint *e = dynamic_cast<IModelTrackedPoint *>(t->getLastChild());
			if (e->getValid())
				_ofs << (trajNumber>0 ? "," : "") << e->getId()
					<< (e->hasTime() ? "," + std::to_string(e->getTime()) : "")
					<< (e->hasX() ? "," + std::to_string(e->getX()) : "")
					<< (e->hasY() ? "," + std::to_string(e->getY()) : "")
					<< (e->hasZ() ? "," + std::to_string(e->getZ()) : "")
					<< (e->hasW() ? "," + std::to_string(e->getW()) : "")
					<< (e->hasH() ? "," + std::to_string(e->getH()) : "")
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
