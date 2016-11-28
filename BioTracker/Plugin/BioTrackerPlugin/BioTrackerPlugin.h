#ifndef BIOTRACKERPLUGIN_H
#define BIOTRACKERPLUGIN_H

#include "QObject"
#include "biotrackerplugin_global.h"

#include "Interfaces/IBioTrackerTrackingInterface.h"


#include "QPointer"
#include "Interfaces/IView/IView.h"
#include "Interfaces/IModel/imodel.h"
#include "Interfaces/IBioTrackerContext.h"
#include "Interfaces/ENUMS.h"

class BIOTRACKERPLUGINSHARED_EXPORT BioTrackerPlugin : public QObject, IBioTrackerTrackingInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "de.fu-berlin.mi.biorobotics.BioTrackerPlugin" FILE "BioTrackerPlugin.json")
    Q_INTERFACES(IBioTrackerTrackingInterface)

public:
    BioTrackerPlugin();


    // IBioTrackerTrackingInterface interface
public:
    void createComponents() override;
    void connectComponents() override;
    void addView(IView *view) override;
    void addModel(IModel *model) override;
    IModel *getModel() override;
    IView *getView() override;
    ENUMS::CONTROLLERTYPE getControllerType() override;
    IBioTrackerContext *getBioTrackerContext() override;


protected:
    void createModel() override;
    void createView() override;
    void connectModelController() override;
    void connectController() override;

Q_SIGNALS:
    void emitCvMatA(std::shared_ptr<cv::Mat> mat, QString name);

public Q_SLOTS:
    void receiveCvMatA(std::shared_ptr<cv::Mat> mat, QString name);

private:
    QPointer< IBioTrackerContext > m_BioTrackerContext;

    IView *m_View;
    IModel *m_Model;

    ENUMS::CONTROLLERTYPE m_ControllerType;

    // IBioTrackerTrackingInterface interface
public:
    void addBioTrackerContext(IBioTrackerContext *context) override;
};

#endif // BIOTRACKERPLUGIN_H
