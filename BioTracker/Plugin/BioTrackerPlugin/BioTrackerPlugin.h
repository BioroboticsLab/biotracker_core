#ifndef BIOTRACKERPLUGIN_H
#define BIOTRACKERPLUGIN_H

#include "biotrackerplugin_global.h"
#include "opencv2/core/core.hpp"
#include "Interfaces/IBioTrackerContext.h"

#include "Interfaces/IBioTrackerPlugin.h"


#include "QPointer"
#include "memory"

class BIOTRACKERPLUGINSHARED_EXPORT BioTrackerPlugin : public IBioTrackerPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "de.fu-berlin.mi.biorobotics.BioTrackerPlugin" FILE "BioTrackerPlugin.json")
    Q_INTERFACES(IBioTrackerPlugin)

public:
    BioTrackerPlugin();


    // IBioTrackerPlugin interface
public:
    void createPlugin() override;

private:
    void connectInterfaces() override;

private:
    IController *m_TrackerController;


    // IBioTrackerPlugin interface
signals:
    void emitCvMat(std::shared_ptr<cv::Mat> mat, QString name) override;

public slots:
    void receiveCvMat(std::shared_ptr<cv::Mat> mat) override;

private slots:
    void receiveCvMatFromController(std::shared_ptr<cv::Mat> mat, QString name) override;
};

#endif // BIOTRACKERPLUGIN_H
