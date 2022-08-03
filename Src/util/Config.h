#pragma once

#include "Utility/IConfig.h"
#include <QString>
#include <map>

class Config : public IConfig
{
public:
    int     DisableWizard             = 0;
    int     FrameStride               = 1;
    double  RectificationHeight       = 100;
    double  RectificationWidth        = 100;
    int     AppertureType             = 0;
    QString CsvSeperator              = ";";
    QString CoreConfigFile            = "BiotrackerCore.ini";
    int     VideoCodecUsed            = 0;
    int     DropFrames                = 0;
    int     RecordScaledOutput        = 0;
    int     DataExporter              = 0;
    int     RecordFPS                 = -1;
    int     CameraWidth               = -1;
    int     CameraHeight              = -1;
    double  GpuQp                     = 20;
    int     UseRegistryLocations      = true;
    QString DefaultLocationManualSave = "";
    QString DirPlugins                = IConfig::dataLocation + "/Plugins/";
    QString DirVideos                 = IConfig::dataLocation + "/Videos/";
    QString DirTracks                 = IConfig::dataLocation + "/Tracks/";
    QString DirTrials       = IConfig::dataLocation + "/Tracks/Trials/";
    QString DirScreenshots  = IConfig::dataLocation + "/Screenshots/";
    QString DirTemp         = IConfig::dataLocation + "/temp/";
    QString AreaDefinitions = IConfig::configLocation + "/areas.csv";

    // Temporary CLI configuration
    QString LoadVideo         = "";
    QString UsePlugins        = "";
    QString CfgCustomLocation = "";

    void load(QString dir, QString file = "config.ini") override;
    void save(QString dir, QString file) override;

    static const QString DefaultArena;
};
