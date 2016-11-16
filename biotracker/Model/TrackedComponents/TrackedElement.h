#ifndef TRACKEDELEMENT_H
#define TRACKEDELEMENT_H

#include "QGraphicsItem"
#include "Interfaces/ITrackedComponent.h"
#include "QString"

class TrackedElement : public ITrackedComponent, public QGraphicsItem
{
    Q_OBJECT

  public:
    TrackedElement(QObject *parent = 0, QString name = "n.a.");

    QString getName();

    void setX(int val);
    void setY(int val);

    int getX();
    int getY();

    // ITrackedComponent interface
public:
    void operate();

private:
    QString name;
    int x;
    int y;

    bool pressed;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // TRACKEDELEMENT_H
