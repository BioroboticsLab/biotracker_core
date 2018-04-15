#pragma once

#ifndef TRACER_H
#define TRACER_H

#include "QAbstractGraphicsShapeItem"

class Tracer : public QObject, public QAbstractGraphicsShapeItem {
    Q_OBJECT

	public:
        Tracer(QVariant type, int frame, float orientation, QPointF pos, float w, float h, QPen pen, QBrush brush, QAbstractGraphicsShapeItem* parent);
        ~Tracer();
    signals:
        void emitGoToFrame(int frame);

public slots:
        void goToFrame();

    protected:
		QRectF boundingRect() const override;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
		void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

		//void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
		//void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
		//void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

        //member
        QString _type;
        int _frame;
        float _orientation;
        QPointF pos;
        float _w;
        float _h;
};



#endif // TRACER_H