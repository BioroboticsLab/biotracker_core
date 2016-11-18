#ifndef IGRAPHICSPIXMAPITEM_H
#define IGRAPHICSPIXMAPITEM_H

#include "IView.h"
#include "QObject"
#include "QGraphicsPixmapItem"

class IGraphicsPixmapItem : public QObject, public QGraphicsPixmapItem, public IView
{
    Q_OBJECT
public:
    IGraphicsPixmapItem(QObject *parent = 0, IController *controller = 0, IModel *model = 0);

    // IView interface
public:
    void setNewModel(IModel *model) override;

public Q_SLOTS:
  virtual void getNotified() = 0;
};

#endif // IGRAPHICSPIXMAPITEM_H
