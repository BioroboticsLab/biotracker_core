#include "GraphicsView.h"
#include "QTimeLine"
#include "QWheelEvent"
#include "../Controller/ControllerGraphicScene.h"

#include <Model/Annotations.h>

#include <qpixmap.h>
#include <QGraphicsItem>
#include <QScrollBar>
#include <QDebug>
#include <QtOpenGL/QGLWidget>
#include <QStringBuilder>

GraphicsView::GraphicsView(QWidget *parent, IController *controller, IModel *model) :
	IViewGraphicsView(parent, controller, model)
{
	m_GraphicsScene = new QGraphicsScene();
	m_BackgroundImage = NULL; 

	this->setScene(m_GraphicsScene);
	//this->setViewport(new QGLWidget());
	
	this->setBackgroundBrush(QBrush(Qt::white));

	this->show();

	setTransformationAnchor(AnchorUnderMouse);
	setViewportUpdateMode(FullViewportUpdate);

	setDragMode(RubberBandDrag);
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
	item->setZValue(-5);

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

void GraphicsView::keyPressEvent(QKeyEvent * event)
{
	emit(onKeyPressEvent(event));
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
	// The middle mouse button is not forwarded but handled here.
	if (event->buttons() & Qt::MidButton)
	{
		m_ViewportDragOrigin = event->pos();
	}
	else
	{
		const QPointF imagePosition = mapToScene(event->pos());
		const QPoint imagePositionInt = QPoint(imagePosition.x(), imagePosition.y());
		event->ignore();
		emit(onMousePressEvent(event, imagePositionInt));
		if (!event->isAccepted())
			QGraphicsView::mousePressEvent(event);
	}
}

void GraphicsView::mouseReleaseEvent(QMouseEvent*event)
{
	if (!(event->buttons() & Qt::MidButton))
	{
		const QPointF imagePosition = mapToScene(event->pos());
		const QPoint imagePositionInt = QPoint(imagePosition.x(), imagePosition.y());
		event->ignore();
		emit(onMouseReleaseEvent(event, imagePositionInt));
		if (!event->isAccepted())
			QGraphicsView::mouseReleaseEvent(event);
	}
}

void GraphicsView::mouseMoveEvent(QMouseEvent*event)
{
	viewport()->update();
	// The middle mouse button is not forwarded but handled here.
	if (event->buttons() & Qt::MidButton)
	{
		const QPoint currentPosition{ event->pos() };
		// Ignore position-reset events..
		if (currentPosition != m_ViewportDragOrigin)
		{
			const QPoint currentPositionScene = currentPosition;
			const QPoint originPositionScene = m_ViewportDragOrigin;
			const auto offset = currentPositionScene - originPositionScene;

			QGraphicsView::horizontalScrollBar()->setValue(horizontalScrollBar()->value() + offset.x() + 1);
			QGraphicsView::verticalScrollBar()->setValue(verticalScrollBar()->value() + offset.y() + 1);
			// Freeze mouse position in place.
			QCursor::setPos(mapToGlobal(m_ViewportDragOrigin));
		}
	}
	else
	{
		const QPointF imagePosition = mapToScene(event->pos());
		const QPoint imagePositionInt = QPoint(imagePosition.x(), imagePosition.y());
		m_cursorPos = imagePositionInt;
		emitCursorPosition(m_cursorPos); //to mainwindow
		event->ignore();
		emit(onMouseMoveEvent(event, imagePositionInt));
		if (!event->isAccepted())
			QGraphicsView::mouseMoveEvent(event);
	}
}

void GraphicsView::receiveToggleAntialiasingFull(bool toggle){
	setRenderHint(QPainter::Antialiasing, toggle);
}