#ifndef TRACKEDELEMENT_H
#define TRACKEDELEMENT_H

#include "QGraphicsItem"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include "QString"

/**
 * This class is an example of how a TrackedComponent could be defined.
 * This class inherits from the IModelTrackedComponent class and is therefor part of the Composite Pattern.
 * This class represents the Leaf class in the Composite Pattern.
 * Objects of this class have a QObject as parent.
 */
class TrackedElement : public IModelTrackedComponent
{
    Q_OBJECT

  public:
    TrackedElement(QObject *parent = 0, QString name = "n.a.", int id=0);

    QString getName();
	void setId(int val) { _id = val; };

    void pressed();
    void notPressed();
    bool getPressedStatus();

    // ITrackedComponent interface
public:
    void operate();

private:
    QString _name;
	int _id;

    bool _pressed;
};

#endif // TRACKEDELEMENT_H
