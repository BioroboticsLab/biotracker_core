#include "GraphicsView.h"
#include "QTimeLine"
#include "QWheelEvent"
#include "../Controller/ControllerGraphicScene.h"

#include <Model/Annotations.h>

#include <qpixmap.h>
#include <QGraphicsItem>

GraphicsView::GraphicsView(QWidget *parent, IController *controller, IModel *model) :
	IViewGraphicsView(parent, controller, model)
{
	m_GraphicsScene = new QGraphicsScene();
	m_BackgroundImage = NULL; //TODO Andi init this

	this->setScene(m_GraphicsScene);

	this->show();
}

void GraphicsView::addGraphicsItem(QGraphicsItem *item)
{
	m_GraphicsScene->addItem(item);

	update();
}

void GraphicsView::removeGraphicsItem(QGraphicsItem *item)
{
	m_GraphicsScene->removeItem(item);

	update();
}

void GraphicsView::addPixmapItem(QGraphicsItem *item)
{
	m_BackgroundImage = item;
	m_GraphicsScene->addItem(item);

	update();
}

void GraphicsView::getNotified()
{

}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
	//if ctrl pressed, use original functionality
	if (event->modifiers() & Qt::ControlModifier)
	{
		QGraphicsView::wheelEvent(event);
	}
	//otherwise, do yours
	else
	{
		if (event->delta() > 0)
		{
			scale(1.1, 1.1);
		}
		else
		{
			scale(0.9, 0.9);
		}
	}
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
	emit(onKeyReleaseEvent(event));
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
	const QPointF imagePosition = mapToScene(event->pos());
	const QPoint imagePositionInt = QPoint(imagePosition.x(), imagePosition.y());
	emit(onMousePressEvent(event, imagePositionInt));
}

void GraphicsView::mouseReleaseEvent(QMouseEvent*event)
{
	const QPointF imagePosition = mapToScene(event->pos());
	const QPoint imagePositionInt = QPoint(imagePosition.x(), imagePosition.y());
	emit(onMouseReleaseEvent(event, imagePositionInt));
}

void GraphicsView::mouseMoveEvent(QMouseEvent*event)
{
	const QPointF imagePosition = mapToScene(event->pos());
	const QPoint imagePositionInt = QPoint(imagePosition.x(), imagePosition.y());
	emit(onMouseMoveEvent(event, imagePositionInt));
}