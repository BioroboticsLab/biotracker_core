#ifndef IBIOTRACKERTRACKINGPLUGIN_H
#define IBIOTRACKERTRACKINGPLUGIN_H

#include "Interfaces/ENUMS.h"
#include "Interfaces/IModel/imodel.h"
#include "Interfaces/IView/IView.h"
#include "Interfaces/IBioTrackerContext.h"

class IBioTrackerTrackingInterface
{
public:
    virtual ~IBioTrackerTrackingInterface() {}

    virtual void createComponents() = 0;
    virtual void connectComponents() = 0;

    virtual void addView(IView *view) = 0;
    virtual void addModel(IModel *model) = 0;
    virtual IModel *getModel() = 0;
    virtual IView *getView() = 0;

    virtual ENUMS::CONTROLLERTYPE getControllerType() = 0;
    virtual IBioTrackerContext *getBioTrackerContext() = 0;

protected:
    virtual void createModel() = 0;
    virtual void createView() = 0;
    virtual void connectModelController() = 0;
    virtual void connectController() = 0;

};

#define BioTrackerTrackingInterface_iid "de.fu-berlin.mi.biorobotics.BioTrackerTrackingInterface"

Q_DECLARE_INTERFACE(BioTracker, BioTrackerTrackingInterface_iid)

#endif // IBIOTRACKERTRACKINGPLUGIN_H
