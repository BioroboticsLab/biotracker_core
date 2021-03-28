#include "Config.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <QStringList>
#include <iostream>
#include <QFile>
#include <QDir>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <QStandardPaths>
#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QTimer>

const QString Config::DefaultArena = "10,10;10,100;100,100;100,10";

template<typename Stream>
Stream &operator>>(Stream& s, QString& q)
{
    std::string tmp;
    s >> tmp;
    q = tmp.data();
	return s;
}

template<typename Stream>
Stream &operator<<(Stream& s, QString const& q)
{
	return s << q.toStdString();
}



void workMap(boost::property_tree::basic_ptree<std::string, std::string> *tree, 
            std::string rep,
            std::map<std::string, std::string> *mp){
                
    auto bakCM = *mp;
    mp->clear();

    for (auto kv : *tree) { 
        if (boost::starts_with(kv.first, rep)){
            for (auto xx : kv.second) { 
                std::string snd = tree->get<std::string>(rep+"."+xx.first);
                std::string fst = xx.first;
                (*mp)[fst]=snd;
            }
        }
    }
    
    if (mp->empty())
        *mp = bakCM;
}

void Config::load(QString dir, QString file)
{
    using namespace boost::property_tree;

    auto tree = ptree{};
    QDir d(dir);
    d.mkpath(dir);
    QFile fin(dir + "/" + file);

    if(!fin.exists()) 
    {       
        fin.open(QIODevice::ReadWrite);
        fin.close();
    }
    try{
        read_ini((dir + "/" + file).toStdString(), tree);
    } 
    catch (std::exception const& e){
            qDebug() << e.what();
            QMessageBox::critical(NULL, QString("Can't read config.ini file!"), QString("%1\n\n%2").arg(e.what(), "\n Closing BioTracker..."), QMessageBox::Ok, QMessageBox::NoButton);
            QTimer::singleShot(250, qApp, SLOT(quit())); //TODO this is hacky! change return value of load() to bool for better quitting
    }
    

    Config* config = this;

    std::string globalPrefix = "General.";
    config->DisableWizard = tree.get<int>(globalPrefix+"DisableWizard",config->DisableWizard);
    config->FrameStride = tree.get<int>(globalPrefix+"FrameStride",config->FrameStride);
    config->RectificationHeight = tree.get<double>(globalPrefix+"RectificationHeight",config->RectificationHeight);
    config->RectificationWidth = tree.get<double>(globalPrefix+"RectificationWidth",config->RectificationWidth);
    config->AppertureType = tree.get<int>(globalPrefix+"AppertureType",config->AppertureType);
    config->CsvSeperator = tree.get<QString>(globalPrefix+"CsvSeperator",config->CsvSeperator);
    config->CoreConfigFile = tree.get<QString>(globalPrefix+"CoreConfigFile",config->CoreConfigFile);
    config->VideoCodecUsed = tree.get<int>(globalPrefix+"VideoCodecUsed",config->VideoCodecUsed);
    config->DropFrames = tree.get<int>(globalPrefix+"DropFrames",config->DropFrames);
    config->RecordScaledOutput = tree.get<int>(globalPrefix+"RecordScaledOutput",config->RecordScaledOutput);
    config->DataExporter = tree.get<int>(globalPrefix+"DataExporter",config->DataExporter);
    config->RecordFPS = tree.get<int>(globalPrefix+"RecordFPS",config->RecordFPS);
    config->CameraWidth = tree.get<int>(globalPrefix+"CameraWidth",config->CameraWidth);
    config->CameraHeight = tree.get<int>(globalPrefix+"CameraHeight",config->CameraHeight);
    config->GpuQp = tree.get<double>(globalPrefix+"GpuQp",config->GpuQp);
    config->DefaultLocationManualSave = tree.get<QString>(globalPrefix+"DefaultLocationManualSave",config->DefaultLocationManualSave);
    config->DirPlugins = tree.get<QString>(globalPrefix+"DirPlugins",config->DirPlugins);
    config->DirVideos = tree.get<QString>(globalPrefix+"DirVideos",config->DirVideos);
    config->DirTracks = tree.get<QString>(globalPrefix+"DirTracks",config->DirTracks);
    config->DirTrials = tree.get<QString>(globalPrefix+"DirTrials",config->DirTrials);
    config->DirScreenshots = tree.get<QString>(globalPrefix+"DirScreenshots",config->DirScreenshots);
    config->DirTemp = tree.get<QString>(globalPrefix+"DirTemp",config->DirTemp);
    config->AreaDefinitions = tree.get<QString>(globalPrefix+"AreaDefinitions",config->AreaDefinitions);
    config->UseRegistryLocations = tree.get<int>(globalPrefix+"UseRegistryLocations",config->UseRegistryLocations);
}

void Config::save(QString dir, QString file)
{
    using namespace boost::property_tree;

    auto tree = ptree{};
    Config *config = this;

    std::string globalPrefix = "General.";
    tree.put(globalPrefix+"DisableWizard", config->DisableWizard);
    tree.put(globalPrefix+"FrameStride", config->FrameStride);
    tree.put(globalPrefix+"RectificationHeight", config->RectificationHeight);
    tree.put(globalPrefix+"RectificationWidth", config->RectificationWidth);
    tree.put(globalPrefix+"AppertureType", config->AppertureType);
    tree.put(globalPrefix+"CsvSeperator", config->CsvSeperator);
    tree.put(globalPrefix+"CoreConfigFile", config->CoreConfigFile);
    tree.put(globalPrefix+"VideoCodecUsed", config->VideoCodecUsed);
    tree.put(globalPrefix+"DropFrames", config->DropFrames);
    tree.put(globalPrefix+"RecordScaledOutput", config->RecordScaledOutput);
    tree.put(globalPrefix+"DataExporter", config->DataExporter);
    tree.put(globalPrefix+"RecordFPS", config->RecordFPS);
    tree.put(globalPrefix+"CameraWidth", config->CameraWidth);
    tree.put(globalPrefix+"CameraHeight", config->CameraHeight);
    tree.put(globalPrefix+"GpuQp", config->GpuQp);
    tree.put(globalPrefix+"DefaultLocationManualSave", config->DefaultLocationManualSave);
    tree.put(globalPrefix+"DirPlugins", config->DirPlugins);
    tree.put(globalPrefix+"DirVideos", config->DirVideos);
    tree.put(globalPrefix+"DirTracks", config->DirTracks);
    tree.put(globalPrefix+"DirTrials", config->DirTrials);
    tree.put(globalPrefix+"DirScreenshots", config->DirScreenshots);
    tree.put(globalPrefix+"DirTemp", config->DirTemp);
    tree.put(globalPrefix+"AreaDefinitions", config->AreaDefinitions);
    tree.put(globalPrefix+"UseRegistryLocations", config->UseRegistryLocations);

    // write_ini((dir + "/" + file).toStdString(), tree);

    try{
        write_ini((dir + "/" + file).toStdString(), tree);
    } 
    catch (std::exception const& e){
            qDebug() << e.what();
            QTimer::singleShot(250, qApp, SLOT(quit()));
    }
}
