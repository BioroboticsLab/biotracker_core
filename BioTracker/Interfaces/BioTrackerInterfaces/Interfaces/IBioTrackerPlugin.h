#ifndef IBIOTRACKERPLUGIN_H
#define IBIOTRACKERPLUGIN_H

#include "QObject"

#include "Interfaces/IController/IController.h"
#include "opencv2/core/core.hpp"
#include "memory"

class IBioTrackerPlugin : public QObject
{
    Q_OBJECT
public:
    ~IBioTrackerPlugin() {}

    virtual void createPlugin() = 0;

    virtual IView *getTrackerParameterWidget() = 0;

private:
    virtual void connectInterfaces() = 0;

Q_SIGNALS:
    virtual void emitCvMat(std::shared_ptr<cv::Mat> mat, QString name) = 0;
    virtual void emitTrackingDone() = 0;

public Q_SLOTS:
    virtual void receiveCvMat(std::shared_ptr<cv::Mat> mat, uint frameNumber) = 0;

private Q_SLOTS:
    virtual void receiveCvMatFromController(std::shared_ptr<cv::Mat> mat, QString name) = 0;
};

#define IBioTrackerPlugin_iid "de.fu-berlin.mi.biorobotics.IBioTrackerPlugin"

Q_DECLARE_INTERFACE(IBioTrackerPlugin, IBioTrackerPlugin_iid)

#endif // IBIOTRACKERPLUGIN_H
