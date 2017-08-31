#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include "Interfaces/IView/IViewGraphicsView.h"
#include "QGraphicsScene"

class GraphicsView : public IViewGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent = 0, IController *controller = 0, IModel *model = 0);

    void addGraphicsItem(QGraphicsItem *item);
    void addPixmapItem(QGraphicsItem *item);
	void removeGraphicsItem(QGraphicsItem *item);
	QGraphicsScene *m_GraphicsScene;//MARKER

    // IGraphicsView interface
public Q_SLOTS:
    void getNotified() override;

    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event) override;

private:

    QGraphicsItem *m_BackgroundImage;

};

#endif // GRAPHICSVIEW_H
