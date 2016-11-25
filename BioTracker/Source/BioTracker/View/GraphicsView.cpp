#include "GraphicsView.h"
#include "QTimeLine"
#include "QWheelEvent"

GraphicsView::GraphicsView(QWidget *parent, IController *controller, IModel *model) :
    IGraphicsView(parent, controller, model)
{
    m_GraphicsScene = new QGraphicsScene();
    this->setScene(m_GraphicsScene);

    this->show();

}

void GraphicsView::addGraphicsItem(QGraphicsItem *item)
{
    m_GraphicsScene->addItem(item);

    update();
}

void GraphicsView::addPixmapItem(QGraphicsItem *item)
{
    m_GraphicsScene->addItem(item);

    update();
}

void GraphicsView::getNotified()
{

}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    //if ctrl pressed, use original functionality
        if(event->modifiers() & Qt::ControlModifier)
        {
            QGraphicsView::wheelEvent(event);
        }
        //otherwise, do yours
        else
        {
           if(event->delta() > 0)
           {
               scale(1.1,1.1);
           }
           else
           {
               scale(0.9,0.9);
           }
       }
}
