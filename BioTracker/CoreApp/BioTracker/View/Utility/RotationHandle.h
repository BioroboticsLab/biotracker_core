#pragma once

#ifndef ROTATIONHANDLE_H
#define ROTATIONHANDLE_H

#include "QGraphicsObject"

/**
* This class inherits QAbstractGraphicsShapeItem.
* It is a child of an componentshape and is used to rotate it.
* (the rotation then gets forwarded to the command compontent,
* which then forwards it to the tracking plugin
* It is attached to the end of the orientation line of the component shape
*/
class RotationHandle : public QObject, public QAbstractGraphicsShapeItem {
	Q_OBJECT

public:
	RotationHandle(QPoint origin, QAbstractGraphicsShapeItem* parent = 0);
	~RotationHandle();

	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

	void setAntialiasing(bool toggle);

signals:
	void emitShapeRotation(double angle, bool rotateEntity = false);

private:
	QPoint _origin;					/**< the position in the coordinate frame of the shape object */
	bool m_antialiasing;			/**< if true, antialiasing is enabled */

};
#endif // ROTATIONHANDLE_H