#ifndef IBIOTRACKERPLUGIN_H
#define IBIOTRACKERPLUGIN_H

#include "QObject"

#include "Interfaces/IController/IController.h"
#include "Interfaces/IModel/IModelDataExporter.h"
#include "Interfaces/IModel/IModelAreaDescriptor.h"
#include "opencv2/core/core.hpp"
#include "memory"

class IBioTrackerPlugin : public QObject
{
    Q_OBJECT
public:
    ~IBioTrackerPlugin() {}

    virtual void createPlugin() = 0;

	virtual IView *getTrackerParameterWidget() = 0;
	virtual IView *getTrackerElementsWidget() = 0;
	virtual IModel *getTrackerComponentModel() = 0;
	virtual void sendCorePermissions() = 0;

private:
    virtual void connectInterfaces() = 0;

Q_SIGNALS:
    virtual void emitCvMat(std::shared_ptr<cv::Mat> mat, QString name) = 0;
    virtual void emitTrackingDone(uint framenumber) = 0;
	virtual void emitChangeDisplayImage(QString str) = 0;
	virtual void emitCorePermission(std::pair<ENUMS::COREPERMISSIONS, bool>) = 0;

public Q_SLOTS:
    virtual void receiveCurrentFrameFromMainApp(std::shared_ptr<cv::Mat> mat, uint frameNumber) = 0;
	virtual void receiveAreaDescriptor(IModelAreaDescriptor *areaDescr) = 0;

private Q_SLOTS:
    virtual void receiveCvMatFromController(std::shared_ptr<cv::Mat> mat, QString name) = 0;
};

#define IBioTrackerPlugin_iid "de.fu-berlin.mi.biorobotics.IBioTrackerPlugin"

Q_DECLARE_INTERFACE(IBioTrackerPlugin, IBioTrackerPlugin_iid)

#endif // IBIOTRACKERPLUGIN_H
