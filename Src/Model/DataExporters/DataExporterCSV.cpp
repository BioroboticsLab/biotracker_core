#include "DataExporterCSV.h"
#include "util/types.h"
#include "Utility/misc.h"
#include <qdebug.h>
#include <qfile.h>
#include <qdatetime.h>


using namespace BioTrackerUtilsMisc; //split

DataExporterCSV::DataExporterCSV(QObject *parent) :
    DataExporterGeneric(parent)
{
    _root = nullptr;
    _separator = _cfg->CsvSeperator.toStdString();
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
            if (!comp->getValid()) {
                ss << _separator;
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
        traj = static_cast<IModelTrackedTrajectory*>(factory->getNewTrackedTrajectory("0"));
        traj->setValid(false);
        traj->add(child, idx);
        root->add(traj, child->getId());
    }
    
    traj->setValid(child->getValid());
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
    int headerEls = static_cast<int>(headerElsStr.size());

    std::vector<std::string> strs;
    split(line, strs, _separator[0]);
    int idcnt = (static_cast<int>(strs.size()) - 2) / headerEls;

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

    //_ofs = QTextStream(_oFileTmp);

    if (!_root) {
        qDebug() << "CORE:  No output opened!";
        return;
    }

    //TODO there is some duplicated code here
    _ofs << QString::fromStdString(std::to_string(idx))
        << QString::fromStdString(_separator) + QString::fromStdString(std::to_string((long long)((((double)idx) / _fps) * 1000)));

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
                _ofs << QString::fromStdString(writeComponentCSV(e, trajNumber));
            }
            trajNumber++;
        }
    }
    //qDebug() << "line written into temp file";
    //_ofs << std::endl;
    _ofs << endl;
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
        qDebug() << "CORE: No output opened!";
        return;
    }
    /*
    if (_ofs.is_open()) {
        _ofs.close();
    }
    */
   if (_oFileTmp->isOpen()) {
        _oFileTmp->close();
   }

    //Find max length of all tracks
    int max = getMaxLinecount();

    //There is nothing to write
    if (max <= 0)
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

    //std::this_thread::sleep_for (std::chrono::seconds(1));

    /*
    //Create final file
    std::ofstream o; 
    o.exceptions(std::ios::failbit | std::ios::badbit);

    if(o.good()){
        qDebug() << "good";
    }
    else{
        qDebug() << "bad";
    }
    */

    //QT-Ansatz, weil std ofstream oft hängt bei open 
    QFile outFile(QString::fromStdString(target));
    outFile.open(QIODevice::WriteOnly);


    if(!outFile.isOpen()){
        qWarning() << "File could not be opened! " << QString::fromStdString(target);
    }

    QTextStream o(&outFile);

    /*
    try {
        //target = "C:/Users/Jonas/Desktop/blubb.csv";
        target = "C:/Users/Jonas/AppData/Roaming/FU Berlin/BioTracker/Tracks/blubbbbbb.csv";
        o.open(target, std::ofstream::out | std::ofstream::app); // hier ist ein fehler im batch processing
    }
    catch (std::ofstream::failure e) {
        qDebug() << "Exception opening/reading file";
    }
    */

    //write metadata
    ControllerDataExporter *ctr = dynamic_cast<ControllerDataExporter*>(_parent);
    SourceVideoMetadata d = ctr->getSourceMetadata();
    o << "# Source name: " << QString::fromStdString(d.name) << endl;
    o << "# Source FPS: " << QString::fromStdString(d.fps) << endl;
    QVariant vv(QDateTime::currentDateTime());
    o << "# Generation time: " << QString::fromStdString(vv.toString().toStdString()) << endl;

    //write header
    int vcount = _root->validCount();
    IModelTrackedComponentFactory* factory = ctr ? ctr->getComponentFactory() : nullptr;
    int headerCount = 0;
    if (factory != nullptr) {
        IModelTrackedComponent *ptraj = static_cast<IModelTrackedComponent*>(factory->getNewTrackedElement("0"));
        std::string header = getHeader(ptraj, vcount);
        headerCount = static_cast<int>(getHeaderElements(ptraj).size());
        o << QString::fromStdString(header) << QString("\n");
        delete ptraj;
    }


    //Write out everything to a new file
    int trajNumber = 0;

    //idx is the frame number
    for (int idx = 0; idx < max; idx++) {

        o << QString::fromStdString(std::to_string(idx))
            << QString::fromStdString(_separator) + QString::fromStdString(std::to_string((((float)idx) / _fps) * 1000));

        int linecnt = 0;
        //i is the track number
        for (int i = 0; i < _root->size(); i++) {

            IModelTrackedTrajectory *t = dynamic_cast<IModelTrackedTrajectory *>(_root->getChild(i));
            if (t && t->validCount() > 0) {
                IModelTrackedPoint *e = dynamic_cast<IModelTrackedPoint*>(t->getChild(idx));
                if (e) {
                    std::string line = writeComponentCSV(e, trajNumber);
                    o << QString::fromStdString(line);
                    linecnt++;
                }
                trajNumber++;
            }
        }
        int count = _root->validCount();
        while (linecnt < count) {
            for (int i = 0; i<headerCount; i++)
                o << QString::fromStdString(_separator);
            linecnt++;
        }
        //o << std::endl;
        o << endl;
    }
    //o.close();
    outFile.close();

    qDebug() << "CORE: Tracks saved in: " << QString::fromStdString(target);
}

void DataExporterCSV::close() {
    /*
    if(_ofs.is_open()){
        _ofs.close();
    }
    */
    if(_oFileTmp->isOpen()){
        _oFileTmp->close();
    }
    //Remove temporary file
    QFile file(_tmpFile.c_str());
    file.remove();
    _oFileTmp->remove();
}
