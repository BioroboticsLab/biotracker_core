#include "DataExporterCSV.h"
#include "Controller/ControllerDataExporter.h"
#include "util/types.h"
#include "util/misc.h"
#include <qdebug.h>
#include <qfile.h>
#include <qdatetime.h>

DataExporterCSV::DataExporterCSV(QObject *parent) :
    DataExporterGeneric(parent)
{
    _root = 0;
    BioTracker::Core::Settings *settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
    _separator = settings->getValueOrDefault<std::string>(CFG_SER_CSVSEP, CFG_SER_CSVSEP_VAL);
}


DataExporterCSV::~DataExporterCSV()
{
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

    return ret;
}

std::string DataExporterCSV::getHeader(IModelTrackedComponent *comp, int cnt) {
    std::stringstream ss;

    ss << "FRAME" << _separator << "MillisecsByFPS";
    for (int c = 0; c < cnt; c++) {
        for (int i = 0; i<comp->metaObject()->propertyCount(); ++i)
        {
            if (comp->metaObject()->property(i).isStored(comp))
            {
                if (!comp->metaObject()->property(i).isStored()) {
                    continue;
                }
                ss << _separator << comp->metaObject()->property(i).name();
            }
        }
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
            ss << _separator << val;
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
        IModelTrackedTrajectory *ntraj = static_cast<IModelTrackedTrajectory*>(factory->getNewTrackedTrajectory("0"));
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
    IModelTrackedComponent* dummy = static_cast<IModelTrackedComponent*>(factory->getNewTrackedElement("0"));
    std::vector<std::string> headerElsStr = getHeaderElements(dummy);
    int headerEls = headerElsStr.size();

    std::vector<std::string> strs;
    split(line, strs, _separator[0]);
    int idcnt = (strs.size() - 2) / headerEls;

    //Add data lines
    while (!ifs.eof()) {

        getline(ifs, line);
        std::vector<std::string> strs;
        split(line, strs, _separator[0]);
        if (strs.size() < 2)
            continue;

        //First two entries are the "global header" (trajectory info, same for all at current timeslice)
        int frame = atoi(strs[0].c_str());
        float frameById = atof(strs[1].c_str());

        //the current trajectory/element pair
        int curTrajCnt = 0;
        IModelTrackedComponent* comp = static_cast<IModelTrackedComponent*>(factory->getNewTrackedElement("0"));

        for (int x = 2; x < strs.size(); x++) {
            setProperty(comp, headerElsStr[curTrajCnt], strs[x].c_str());

            curTrajCnt++;
            if (curTrajCnt >= headerEls) {
                curTrajCnt = 0;
                addChildOfChild(_root, comp, factory, frame);
                comp = static_cast<IModelTrackedComponent*>(factory->getNewTrackedElement("0"));
            }
        }
        //addChildOfChild(_root, comp, factory, frame);
    }
}

void DataExporterCSV::write(int idx) {
    if (!_root) {
        qDebug() << "No output opened!";
        return;
    }

    //TODO there is some duplicated code here
    _ofs << std::to_string(idx)
        << _separator + std::to_string((long long)((((double)idx) / _fps) * 1000));

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
    cleanup();
    open(_root);
}

void DataExporterCSV::writeAll(std::string f) {
    //Sanity
    if (!_root) {
        qDebug() << "No output opened!";
        return;
    }
    if (_ofs.is_open()) {
        _ofs.close();
    }

    //Find max length of all tracks
    int max = getMaxLinecount();

    //There is nothing to write
    if (max <= 1)
    {
        cleanup();
        return;
    }

    std::string target = f;
    if (target.size() <= 4) {
        target = _finalFile;
    }
    if (target.substr(target.size() - 4) != ".csv")
        target += ".csv";

    //Create final file
    std::ofstream o; 
    o.open(target, std::ofstream::out);

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
    int headerCount = 0;
    if (factory != nullptr) {
        IModelTrackedComponent *ptraj = static_cast<IModelTrackedComponent*>(factory->getNewTrackedElement("0"));
        std::string header = getHeader(ptraj, vcount);
        headerCount = getHeaderElements(ptraj).size();
        o << header << "\n";
        delete ptraj;
    }


    //Write out everything to a new file
    int trajNumber = 0;

    //idx is the frame number
    for (int idx = 0; idx < max; idx++) {

        o << std::to_string(idx)
            << _separator + std::to_string((((float)idx) / _fps) * 1000);

        int linecnt = 0;
        //i is the track number
        for (int i = 0; i < _root->size(); i++) {

            IModelTrackedTrajectory *t = dynamic_cast<IModelTrackedTrajectory *>(_root->getChild(i));
            if (t) {
                IModelTrackedPoint *e = dynamic_cast<IModelTrackedPoint*>(t->getChild(idx));
                if (e) {
                    std::string line = writeComponentCSV(e, trajNumber);
                    o << line;
                    linecnt++;
                }
                trajNumber++;
            }
        }
        int count = _root->validCount();
        while (linecnt < count) {
            for (int i = 0; i<headerCount; i++)
                o << _separator;
            linecnt++;
        }
        o << std::endl;
    }
    o.close();
}

void DataExporterCSV::close() {
    _ofs.close();

    if (!_root || _root->size() == 0) {
        //Remove temporary file
        QFile file(_tmpFile.c_str());
        file.remove();
    }
}
