#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include <QObject>
#include "QMap"
#include "QPointer"

#include <boost/filesystem.hpp>

#include "Interfaces/IView/IView.h"
#include "Interfaces/IModel/imodel.h"
#include "Interfaces/IBioTrackerContext.h"
#include "Interfaces/ENUMS.h"


class IController : public QObject {
    Q_OBJECT
  public:
    explicit IController(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);

    void createComponents();
    void connectComponents();

    void addView(IView *view);
    void addModel(IModel *model);
    IModel *getModel();
    IView *getView();

    ENUMS::CONTROLLERTYPE getControllerType();
    IBioTrackerContext *getBioTrackerContext();

protected:
    virtual void createModel() = 0;
    virtual void createView() = 0;
    virtual void connectModelController() = 0;
    virtual void connectController() = 0;

protected:
    QPointer< IBioTrackerContext > m_BioTrackerContext;

    IView *m_View;
    QPointer<IModel> m_Model;

    ENUMS::CONTROLLERTYPE m_ControllerType;

};

#define IController_iid "de.fu-berlin.mi.biorobotics.IController"

Q_DECLARE_INTERFACE(IController, IController_iid)

#endif // ICONTROLLER_H
