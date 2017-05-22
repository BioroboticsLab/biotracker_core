#ifndef TRACKEDELEMENT_H
#define TRACKEDELEMENT_H

#include "QGraphicsItem"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include "QString"
#include "fish\pose\FishPose.h"

/**
 * This class is an example of how a Tracked Element could be defined.
 * This class inherits from the IModelTrackedComponent class and is therefor part of the Composite Pattern.
 * This class represents the Leaf class as stated by the GoF.
 * Objects of this class can have a QObject as parent and as it appears in this example have a name.
 */
class TrackedElement : public IModelTrackedComponent
{
    Q_OBJECT

  public:
    TrackedElement(QObject *parent = 0, QString name = "n.a.", int id=0);

    QString getName();

    void setFishPose(FishPose p);
	void setId(int val) { _id = val; };

    void pressed();
    void notPressed();

	FishPose getFishPose();
	int getId() { return _id; };
    bool getPressedStatus();

    // ITrackedComponent interface
public:
    void operate();

private:
    QString _name;
	FishPose _pose;
	int _id;

    bool _pressed;
};

#endif // TRACKEDELEMENT_H
