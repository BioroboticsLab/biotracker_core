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

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent*event) override;
	void mouseMoveEvent(QMouseEvent*event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void enterEvent(QEvent *event) override;

    // IGraphicsView interface
public Q_SLOTS:
    void getNotified() override;
	void receiveToggleAntialiasingFull(bool toggle);

    // QWidget interface
protected:
	void wheelEvent(QWheelEvent *event) override;

private:
    QGraphicsItem *m_BackgroundImage;
	QPoint m_ViewportDragOrigin{ 0, 0 };
	QPoint m_cursorPos;

Q_SIGNALS:
	// If you connect to these signals, you MUST use Qt::DirectConnection.
	// You also SHOULD call event->accept() if you handle it.
	// You MAY want to only handle events with isAccepted() == false.
	void onMousePressEvent(QMouseEvent *event, QPoint imagePosition);
	void onMouseReleaseEvent(QMouseEvent*event, QPoint imagePosition);
	void onMouseMoveEvent(QMouseEvent*event, QPoint imagePosition);
	void onKeyReleaseEvent(QKeyEvent *event);
	void onKeyPressEvent(QKeyEvent *event);

	void emitCursorPosition(QPoint pos);


};

#endif // GRAPHICSVIEW_H
