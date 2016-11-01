#ifndef TRACKEDELEMENT_H
#define TRACKEDELEMENT_H

#include "Interfaces/ITrackedComponent.h"
#include "QString"

class TrackedElement : public ITrackedComponent
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
};

#endif // TRACKEDELEMENT_H
