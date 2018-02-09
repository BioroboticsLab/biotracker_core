
#ifndef ICONTROLLER_H
#define ICONTROLLER_H

#include <QObject>
#include "QMap"
#include "QPointer"

#include "Interfaces/IView/IView.h"
#include "Interfaces/IModel/IModel.h"
#include "Interfaces/IBioTrackerContext.h"
#include "Interfaces/ENUMS.h"


class IController : public QObject {
    Q_OBJECT
  public:
    explicit IController(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);

    /**
     * @brief Stores the current zoom and pan offsets. While panning, panState stores the last mouse cursor position.
     */
    void createComponents();

    /**
     * @brief connectComponents
     */
    void connectComponents();

    virtual void cleanup();

    void addView(IView *view);
    void addModel(IModel *model);
    IModel *getModel();
    IView *getView();

    ENUMS::CONTROLLERTYPE getControllerType();
    IBioTrackerContext *getBioTrackerContext();

protected:
    virtual void createModel() = 0;
    virtual void createView() = 0;
    virtual void connectModelToController() = 0;
    virtual void connectControllerToController() = 0;

protected:
    QPointer< IBioTrackerContext > m_BioTrackerContext;

	IView *m_View;
    QPointer<IModel> m_Model;

    ENUMS::CONTROLLERTYPE m_ControllerType;

};

#endif // ICONTROLLER_H
