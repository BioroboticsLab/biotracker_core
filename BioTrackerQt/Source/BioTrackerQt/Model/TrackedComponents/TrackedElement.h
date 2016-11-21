#ifndef TRACKEDELEMENT_H
#define TRACKEDELEMENT_H

#include "QGraphicsItem"
#include "Interfaces/IModel/ITrackedComponent.h"
#include "QString"

class TrackedElement : public ITrackedComponent
{
    Q_OBJECT

  public:
    TrackedElement(QObject *parent = 0, QString name = "n.a.");

    QString getName();

    void setX(int val);
    void setY(int val);
    void pressed();
    void notPressed();

    int getX();
    int getY();
    bool getPressedStatus();

    // ITrackedComponent interface
public:
    void operate();

private:
    QString name;
    int x;
    int y;

    bool mPressed;
};

#endif // TRACKEDELEMENT_H
