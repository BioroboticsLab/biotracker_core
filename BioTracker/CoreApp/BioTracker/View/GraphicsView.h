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

	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent*event);
	void mouseMoveEvent(QMouseEvent*event);
	void keyReleaseEvent(QKeyEvent *event);

    // IGraphicsView interface
public Q_SLOTS:
    void getNotified() override;

    // QWidget interface
protected:
	void wheelEvent(QWheelEvent *event) override;

private:
    QGraphicsItem *m_BackgroundImage;

Q_SIGNALS:
	void onMousePressEvent(QMouseEvent *event, QPoint imagePosition);
	void onMouseReleaseEvent(QMouseEvent*event, QPoint imagePosition);
	void onMouseMoveEvent(QMouseEvent*event, QPoint imagePosition);
	void onKeyReleaseEvent(QKeyEvent *event);
};

#endif // GRAPHICSVIEW_H
