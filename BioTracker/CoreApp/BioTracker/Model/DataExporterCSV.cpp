#include "DataExporterCSV.h"
#include "Controller/ControllerDataExporter.h"
#include "util/types.h"
#include "util/misc.h"
#include <qdebug.h>
#include <qfile.h>
#include <qdatetime.h>

DataExporterCSV::DataExporterCSV(QObject *parent) :
    IModelDataExporter(parent)
{
    _parent = parent;
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

}

//https://codereview.stackexchange.com/questions/29611/finding-the-number-of-occurrences-of-a-string-in-another-string
int DataExporterCSV::key_search(const std::string& s, const std::string& key)
{
    int count = 0;
    size_t pos = 0;
    while ((pos = s.find(key, pos)) != std::string::npos) {
        ++count;
        ++pos;
    }
    return count;
}


std::vector<std::string> DataExporterCSV::getHeaderElements(IModelTrackedComponent *comp) {
    std::vector<std::string> ret;

    for (int i = 0; i<comp->metaObject()->propertyCount(); ++i)
    {
        if (comp->metaObject()->property(i).isStored(comp))
        {
            if (!comp->metaObject()->property(i).isStored()) {
                continue;
            }
            ret.push_back(comp->metaObject()->property(i).name());
        }
    }
    //IModelComponentTemporal* temp = dynamic_cast<IModelComponentTemporal*>(comp);
    //if (temp) {
    //    ret.push_back("Millisecs");
    //    ret.push_back("ReadableTime");
    //}

    return ret;
}

std::string DataExporterCSV::getHeader(IModelTrackedComponent *comp, int cnt) {
    std::stringstream ss;

    ss << "FRAME,MillisecsByFPS";
    for (int c = 0; c < cnt; c++) {
        for (int i = 0; i<comp->metaObject()->propertyCount(); ++i)
        {
            if (comp->metaObject()->property(i).isStored(comp))
            {
                if (!comp->metaObject()->property(i).isStored()) {
                    continue;
                }
                ss << "," << comp->metaObject()->property(i).name();
            }
        }
        //IModelComponentTemporal* temp = dynamic_cast<IModelComponentTemporal*>(comp);
        //if (temp) {
        //    ss << ",Millisecs,ReadableTime";
        //}
    }

    std::string s = ss.str();
    return s;
}

/* Writes a tracked component to string
*/
std::string DataExporterCSV::writeComponentCSV(IModelTrackedComponent* comp, int tid) {
    std::stringstream ss;

    for (int i = 0; i<comp->metaObject()->propertyCount(); ++i)
    {
        if (comp->metaObject()->property(i).isStored(comp))
        {
            if (!comp->metaObject()->property(i).isStored()) {
                continue;
            }
            std::string str = comp->metaObject()->property(i).name();
            QVariant v = comp->metaObject()->property(i).read(comp);
            std::string val = v.toString().toStdString();
            val = (val == "" ? "0" : val);
            ss << "," << val;
        }
    }

    return ss.str();
}

void DataExporterCSV::setProperty(IModelTrackedComponent* comp, std::string key, std::string val) {
    QString qval = QString(val.c_str());
    QVariant v(qval);
    comp->setProperty(key.c_str(), v);
}

void DataExporterCSV::addChildOfChild(IModelTrackedTrajectory *root, IModelTrackedComponent* child, IModelTrackedComponentFactory* factory, int idx) {
    IModelTrackedTrajectory *traj = dynamic_cast<IModelTrackedTrajectory *>(root->getChild(child->getId()));
    if (traj) {
        traj->add(child, idx);
    }
    else
    {
        IModelTrackedTrajectory *ntraj = static_cast<IModelTrackedTrajectory*>(factory->getNewTrackedTrajectory());
        ntraj->add(child, idx);
        root->add(ntraj, idx);
    }
}

void DataExporterCSV::loadFile(std::string file)
{
    std::ifstream ifs(file, std::ifstream::in);

    std::string line = "# ";
    while (line.substr(0, 1) == "#") {
        getline(ifs, line);
    }

    //parse header
    ControllerDataExporter *ctr = dynamic_cast<ControllerDataExporter*>(_parent);
    IModelTrackedComponentFactory* factory = ctr ? ctr->getComponentFactory() : nullptr;
    if (!factory) {
        return;
    }
    IModelTrackedComponent* dummy = static_cast<IModelTrackedComponent*>(factory->getNewTrackedElement());
    std::vector<std::string> headerElsStr = getHeaderElements(dummy);
    int headerEls = headerElsStr.size();

    std::vector<std::string> strs;
    split(line, strs, ',');
    int idcnt = (strs.size() - 2) / headerEls;

    //Add data lines
    while (!ifs.eof()) {

        getline(ifs, line);
        std::vector<std::string> strs;
        split(line, strs, ',');
        if (strs.size() < 2)
            continue;

        //First two entries are the "global header" (trajectory info, same for all at current timeslice)
        int frame = atoi(strs[0].c_str());
        float frameById = atof(strs[1].c_str());

        //the current trajectory/element pair
        int curTrajCnt = 0;
        IModelTrackedComponent* comp = static_cast<IModelTrackedComponent*>(factory->getNewTrackedElement());

        for (int x = 2; x < strs.size(); x++) {
            setProperty(comp, headerElsStr[curTrajCnt], strs[x].c_str());

            curTrajCnt++;
            if (curTrajCnt >= headerEls) {
                curTrajCnt = 0;
                addChildOfChild(_root, comp, factory, frame);
                comp = static_cast<IModelTrackedComponent*>(factory->getNewTrackedElement());
            }
        }
        addChildOfChild(_root, comp, factory, frame);
    }

    /* Some testcode to generate a large amount of fake-data
    ControllerDataExporter *ctr = dynamic_cast<ControllerDataExporter*>(_parent);
    IModelTrackedComponentFactory* factory = ctr ? ctr->getComponentFactory() : nullptr;
    if (!factory) {
    return;
    }
    for (int x = 3; x < 5; x++) {

    IModelTrackedTrajectory *child = static_cast<IModelTrackedTrajectory*>(factory->getNewTrackedTrajectory());;
    for (int i = 0; i < 1000000; i++) {
    IModelTrackedComponent *node = static_cast<IModelTrackedComponent*>(factory->getNewTrackedElement());
    IModelComponentEuclidian2D *n = dynamic_cast<IModelComponentEuclidian2D*>(node);
    n->setXpx(100);
    n->setYpx(100);
    n->setRad(1);
    n->setDeg(100);
    n->setValid(true);
    n->setId(i);
    child->add(node, i);
    }
    _root->add(child, x);
    }
    */
}

void DataExporterCSV::write(int idx) {
    if (!_root) {
        qDebug() << "No output opened!";
        return;
    }

    //TODO there is some duplicated code here
    _ofs << std::to_string(idx)
        << "," + std::to_string((long long)((((double)idx) / _fps) * 1000));

    //Write single trajectory
    int trajNumber = 0;
    for (int i = 0; i < _root->size(); i++) {

        IModelTrackedTrajectory *t = dynamic_cast<IModelTrackedTrajectory *>(_root->getChild(i));
        if (t) {
            IModelTrackedPoint *e;
            if (idx == -1)
                e = dynamic_cast<IModelTrackedPoint*>(t->getLastChild());
            else
                e = dynamic_cast<IModelTrackedPoint*>(t->getChild(idx));
            if (e && e->getValid()) {
                _ofs << writeComponentCSV(e, trajNumber);
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

    //write metadata
    ControllerDataExporter *ctr = dynamic_cast<ControllerDataExporter*>(_parent);
    SourceVideoMetadata d = ctr->getSourceMetadata();
    o << "# Source name: " << d.name << std::endl;
    o << "# Source FPS: " << d.fps << std::endl;
    QVariant vv(QDateTime::currentDateTime());
    o << "# Generation time: " << vv.toString().toStdString() << std::endl;

    //write header
    int vcount = _root->validCount();
    IModelTrackedComponentFactory* factory = ctr ? ctr->getComponentFactory() : nullptr;
    if (factory != nullptr) {
        IModelTrackedComponent *ptraj = static_cast<IModelTrackedComponent*>(factory->getNewTrackedElement());
        o << getHeader(ptraj, vcount) << "\n";
        delete ptraj;
    }


    //Write out everything to a new file
    int trajNumber = 0;

    //idx is the frame number
    for (int idx = 0; idx < max; idx++) {

        o << std::to_string(idx)
            << "," + std::to_string((((float)idx) / _fps) * 1000);

        //i is the track number
        for (int i = 0; i < _root->size(); i++) {

            IModelTrackedTrajectory *t = dynamic_cast<IModelTrackedTrajectory *>(_root->getChild(i));
            if (t) {
                IModelTrackedPoint *e = dynamic_cast<IModelTrackedPoint*>(t->getChild(idx));
                if (e) {
                    o << writeComponentCSV(e, trajNumber);
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
