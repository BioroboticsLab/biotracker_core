#ifndef BIOTRACKERPLUGIN_H
#define BIOTRACKERPLUGIN_H

#include "biotrackerplugin_global.h"
#include "opencv2/core/core.hpp"
#include "Interfaces/IBioTrackerContext.h"

#include "Interfaces/IBioTrackerPlugin.h"


#include "QPointer"
#include "memory"

class BIOTRACKERPLUGINSHARED_EXPORT BioTrackerPlugin : public IBioTrackerPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "de.fu-berlin.mi.biorobotics.BioTrackerPlugin" FILE "BioTrackerPlugin.json")
    Q_INTERFACES(IBioTrackerPlugin)

  public:
    BioTrackerPlugin();

    // IBioTrackerPlugin interface
    IView* getTrackerParameterWidget();
	IView *getTrackerElementsWidget();
	void setDataExporter(IModelDataExporter *exporter);

  public:
    void createPlugin();
    void receiveCurrentFrameFromMainApp(std::shared_ptr<cv::Mat> mat, uint frameNumber);

  private:
    void connectInterfaces();
  signals:
    void emitCvMat(std::shared_ptr<cv::Mat> mat, QString name);
    void emitTrackingDone();
	void emitChangeDisplayImage(QString str);

private slots:
    void receiveCvMatFromController(std::shared_ptr<cv::Mat> mat, QString name);
    void receiveTrackingDone();
	void receiveChangeDisplayImage(QString str);


private:
	IController *m_TrackerController;
	IController *m_ComponentController;

	IBioTrackerContext *m_PluginContext;

};

#endif // BIOTRACKERPLUGIN_H
