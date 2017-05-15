#ifndef IVIEWTRACKEDCOMPONENT_H
#define IVIEWTRACKEDCOMPONENT_H

#include "IView.h"
#include "QGraphicsObject"

/**
 * This is the common interface for all views representing a TrackedComponents class (IModelTrackedTrajectory and any leaf class).
 * This class is derived from IView from the Model-View-Controller structure and visializes the data from its IModel object.
 * A IViewTrackedComponent has a QGraphicsItem.
 */

class IViewTrackedComponent : public QGraphicsObject, public IView
{
    Q_OBJECT
public:
    /**
     * The constructor hase a QGraphicsItem object as Parent, an IController object as its controller and an IModel object.
     */
    IViewTrackedComponent(QGraphicsItem *parent = 0, IController *controller = 0, IModel *model = 0);
    ~IViewTrackedComponent();

    // IView interface
public:
    void setNewModel(IModel *model) override;

protected:
    void connectModelView() override;

public Q_SLOTS:
  virtual void getNotified() = 0;

};

#endif // IVIEWTRACKEDCOMPONENT_H
