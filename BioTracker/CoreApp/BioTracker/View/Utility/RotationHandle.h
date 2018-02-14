#pragma once

#ifndef ROTATIONHANDLE_H
#define ROTATIONHANDLE_H

#include "QGraphicsObject"


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
		QPoint _origin;
		bool m_antialiasing;

};
#endif // ROTATIONHANDLE_H