#pragma once

#ifndef TRACER_H
    #define TRACER_H

    #include "QAbstractGraphicsShapeItem"

/**
 * This class inherits QAbstractGraphicsShapeItem.
 * It is created by and an child of a componenntshape.
 * It is a helper class used to visualize shape type tracers.
 * It has a context menu to set medium to the tracer's frame
 * Apart from the polygon it is visualized as the componentshapes
 * current type.
 */
class Tracer : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT

public:
    Tracer(QVariant                    type,
           int                         frame,
           float                       orientation,
           QPointF                     pos,
           float                       w,
           float                       h,
           QPen                        pen,
           QBrush                      brush,
           QAbstractGraphicsShapeItem* parent);
    ~Tracer();
signals:
    void emitGoToFrame(int frame);

public slots:

    // signals the media player rewind to the tracer's frame
    void goToFrame();

protected:
    // Interface of QAbstractGraphicsShapeItem
    QRectF boundingRect() const override;
    void   paint(QPainter*                       painter,
                 const QStyleOptionGraphicsItem* option,
                 QWidget*                        widget) override;
    void   contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

    // member
    QString _type;  /**< type visualized */
    int     _frame; /**< corresponding frame  */
    float
        _orientation; /**< orientation of tracer --> of corresponding entity */
    QPointF pos; /**< position in coordinateframe of parent shape object  */
    float   _w;  /**< width  */
    float   _h;  /**< height */
};

#endif // TRACER_H
