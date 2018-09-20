#ifndef ITRACKEDCOMPONENT_H
#define ITRACKEDCOMPONENT_H

#include "IModel.h"
#include <chrono>

#include <QObject>
#include <QDataStream>
#include <QPolygonF>
#include <ostream>


/**
* This is the common interface for all TrackedComponents (IModelTrackedTrajectory and any leaf class).
* It is part of the Composite Pattern and is the equivalent to the abstract Component class.
* This class is derived from IModel from the Model-View-Controller structure.
* A TrackedComponent has a QObject as parent and it
* has a methode called operate() that needs to be implemented
* by all derivatives of this interface class.
*/

/*
*	Blank component
*/

class IModelTrackedComponent : public IModel
{
	Q_OBJECT
	/*
	Q_PROPERTY-System: http://doc.qt.io/qt-5/properties.html
	*/
	Q_PROPERTY(bool valid READ getValid() WRITE setValid STORED true)
	Q_PROPERTY(double id READ getId() WRITE setId STORED true)
public:
	/**
	* The constructor of the IModelTrackedComponent class is able to receive a QObject as parent.
	*/
	IModelTrackedComponent(QObject *parent = 0);
	/*
	Getters, Setters & Checkers
	*/
	virtual void  setValid(bool v) { _valid = v; };
	virtual void  setId(int id) { _id = id; };
    virtual void  setFixed(bool fixed) { _fixed = fixed; };
    virtual void  setParent(IModelTrackedComponent *p) { _parentNode = p; };
	virtual int   getId() { return _id; };
	virtual bool  getValid() { return _valid; };
	virtual bool  getFixed() { return _fixed; };

	/**
	* The methode operate() must be implemented by every derivative of this class.
	*/
	virtual void operate() = 0;

protected:
	int _id;                                       /**< id of the component */
	bool _valid;                                   /**< validity of the component; if invalid -> not visualized */
	bool _fixed;                                   /**< fix status; if fixed the tracked should not change state*/
    IModelTrackedComponent *_parentNode = nullptr; /**< parent */
};


Q_DECLARE_METATYPE(QList<IModelTrackedComponent*>)
/**
* Serialization operator for multiple components.
*/
QDataStream &operator<<(QDataStream &ds, const QList<IModelTrackedComponent*> &data);
/**
* Deserialization operator for multiple components.
*/
QDataStream &operator>>(QDataStream &ds, QList<IModelTrackedComponent*> &data);
/**
* Serialization operator for one component.
*/
QDataStream &operator<<(QDataStream &out, const IModelTrackedComponent &painting);
/**
* Deserialization operator for one component.
*/
QDataStream &operator>>(QDataStream &in, IModelTrackedComponent &painting);

/***************************************************************************//**
* This is a node of the Composite Pattern
* This class is derived from IModelTrackedComponent.
* It includes positional data like x,y-coordinates (in px and _coordinateUnit), the orientation in rad and deg 
* and the width and height (in px and _coordinateUnit)
*******************************************************************************/

class IModelComponentEuclidian2D : public IModelTrackedComponent {
public:
	Q_OBJECT
	/*
	Q_PROPERTY-System: http://doc.qt.io/qt-5/properties.html
	*/
	Q_PROPERTY(QString coordinateUnit READ getCoordinateUnit() WRITE setCoordinateUnit STORED true)
	Q_PROPERTY(float x READ getX() WRITE setX STORED hasX)
	Q_PROPERTY(float y READ getY() WRITE setY STORED hasY)
	Q_PROPERTY(float w READ getW() WRITE setW STORED hasW)
	Q_PROPERTY(float h READ getH() WRITE setH STORED hasH)
	Q_PROPERTY(float wpx READ getWpx() WRITE setWpx STORED hasWpx)
	Q_PROPERTY(float hpx READ getHpx() WRITE setHpx STORED hasHpx)
	Q_PROPERTY(float rad READ getRad() WRITE setRad STORED hasRad)
	Q_PROPERTY(float deg READ getDeg() WRITE setDeg STORED hasDeg)
	Q_PROPERTY(float xpx READ getXpx() WRITE setXpx STORED hasXpx)
	Q_PROPERTY(float ypx READ getYpx() WRITE setYpx STORED hasYpx)

public:
	/**
	* The constructor of the IModelTrackedComponent class is able to receive a QObject as parent.
	* It should not be implemented as it is a not a leaf or a composite
	*/
	IModelComponentEuclidian2D(QObject *parent = 0);

    /* Returns a human-readable string about coordinate units.
    * Freely enter something like "px", "cm" or "um". This will be annotated in the output.
    */
	virtual QString getCoordinateUnit() { return "cm"; };
	virtual void setCoordinateUnit(QString c) { _coordinateUnit = c; };

	/*
	Getters, Setters & Checkers
	*/
    virtual void  setX(float x) = 0;
    virtual void  setY(float y) = 0;
    virtual void  setW(float w) = 0;
    virtual void  setH(float h) = 0;
	virtual void  setXpx(float h) = 0;
	virtual void  setYpx(float h) = 0;
	virtual void  setWpx(float h) = 0;
	virtual void  setHpx(float h) = 0;
    virtual void  setRad(float r) = 0;
    virtual void  setDeg(float d) = 0;

    virtual float getX() = 0;
    virtual float getY() = 0;
    virtual float getW() = 0;
    virtual float getH() = 0;
    virtual float getXpx() = 0;
    virtual float getYpx() = 0;
    virtual float getWpx() = 0;
    virtual float getHpx() = 0;
    virtual float getRad() = 0;
    virtual float getDeg() = 0;

    virtual bool hasX() = 0;
    virtual bool hasY() = 0;
    virtual bool hasW() = 0;
    virtual bool hasH() = 0;
    virtual bool hasXpx() = 0;
    virtual bool hasYpx() = 0;
    virtual bool hasWpx() = 0;
    virtual bool hasHpx() = 0;
    virtual bool hasRad() = 0;
    virtual bool hasDeg() = 0;

protected:
	QString _coordinateUnit;     /**< coordinate unit, will be saved in exported file */
	float _x;                    /**< x-coordinate in coordinate unit*/
	float _y;                    /**< y-coordinate in coordinate unit*/
	float _w;                    /**< width in coordinate unit*/
	float _h;                    /**< height in coordinate unit*/
	float _rad;                  /**< orientation in rad */
	float _deg;                  /**< orientation in deg */
	float _xpx;                  /**< x-coordinate in px */
	float _ypx;                  /**< y-coordinate in px */
	float _wpx;                  /**< width in px */
	float _hpx;                  /**< height in px */
};

/***************************************************************************//**
* This is a node of the Composite Pattern
* This class is derived from IModelComponentEuclidian2D.
* It expands it from 2D to 3D.
* Reserved for future 3D tracking.
*******************************************************************************/
class IModelComponentEuclidian3D : public IModelComponentEuclidian2D {
public:
	Q_OBJECT
	/*
	Q_PROPERTY-System: http://doc.qt.io/qt-5/properties.html
	*/
	Q_PROPERTY(float z     READ getZ()			WRITE setZ          STORED hasZ)
	Q_PROPERTY(float zpx   READ getZpx()		WRITE setZpx        STORED hasZpx)
	Q_PROPERTY(float d     READ getD()			WRITE setD          STORED hasD)
	Q_PROPERTY(float dpx   READ getDpx()	    WRITE setDpx        STORED hasDpx)
	Q_PROPERTY(float rada2 READ getRadAxis2()  WRITE setRadAxis2   STORED hasRadAxis2)
	Q_PROPERTY(float rada3 READ getRadAxis3()  WRITE setRadAxis3   STORED hasRadAxis3)

public:
	/*
	Getters, Setters & Checkers
	*/
	virtual void  setZ(float z) = 0;
	virtual void  setZpx(float z) = 0;
    virtual void  setD(float h) = 0;
	virtual void  setDpx(float d) = 0;
    virtual void  setRadAxis2(float r) = 0;
    virtual void  setRadAxis3(float r) = 0;

	virtual float getZ() = 0;
	virtual float getZpx() = 0;
    virtual float getD() = 0;
    virtual float getDpx() = 0;
    virtual float getRadAxis2() = 0;
    virtual float getRadAxis3() = 0;

    virtual bool hasZ() = 0;
    virtual bool hasD() = 0;
    virtual bool hasZpx() = 0;
    virtual bool hasDpx() = 0;
    virtual bool hasRadAxis2() = 0;
    virtual bool hasRadAxis3() = 0;

protected:
	float _z;                        /**< z-coordinate in coordinate unit */
	float _zpx;                      /**< z-coordinate in px */
	float _d;                        /**< d in coordinate unit*/
	float _dpx;                      /**< d in px */
	float _rada2;                    /**< rotation two */
	float _rada3;                    /**< rotation three */
};

/***************************************************************************//**
* This is a node of the Composite Pattern
* This class is derived from IModelComponentEuclidian2D.
* It expands it to 2D with timestamps.
*******************************************************************************/
class IModelComponentTemporal2D : public IModelComponentEuclidian2D {
public:
    Q_OBJECT 
	/*
	Q_PROPERTY-System: http://doc.qt.io/qt-5/properties.html
	*/
    Q_PROPERTY(qint64 time READ getTime() WRITE setTime STORED hasTime)
    Q_PROPERTY(QString timeString READ getTimeString() WRITE setTimeString STORED hasTimeString)

public:
	/**
	* The constructor of the IModelComponentTemporal2D class is able to receive a QObject as parent.
	* It should not be implemented as it is a not a leaf nore a composite
	*/
    IModelComponentTemporal2D(QObject *parent = 0);

	/*
	Getters, Setters & Checkers
	*/
    virtual void setTime(qint64 t) = 0;
    virtual qint64 getTime() = 0;
    virtual bool hasTime() = 0;
    virtual void setTimeString(QString t) = 0;
    virtual QString getTimeString() = 0;
    virtual bool hasTimeString() = 0;

protected:
    qint64 _time;                                /**< UNIX timestamp */
    QString _timeString;                         /**< timestamp a formatted as string */
};

/***************************************************************************//**
* This is a leaf of the Composite Pattern
* This class is derived from IModelComponentTemporal2D.
* It includes 2D and time data.
* This interface can be visualized in the core application if the tracking data implements it.
* A circular point will be visualized with the radius of min(width,height)
* The position is the center of the point.
*******************************************************************************/
class IModelTrackedPoint : public IModelComponentTemporal2D
{
	Q_OBJECT

public:
	/**
	* The constructor of the IModelTrackedPoint class is able to receive a QObject as parent.
	*/
	IModelTrackedPoint(QObject *parent = 0);
};

/***************************************************************************//**
* This is a leaf of the Composite Pattern
* This class is derived from IModelComponentTemporal2D.
* It includes 2D and time data and a polygon.
* This interface can be visualized in the core application if the tracking data implements it.
* An irregular polygon defined by a list of polygons will be visualized.
*******************************************************************************/

class IModelTrackedPolygon : public IModelComponentTemporal2D
{
	Q_OBJECT

public:

	/**
	* The constructor of the IModelTrackedPolygon class is able to receive a QObject as parent.
	*/
	IModelTrackedPolygon(QObject *parent = 0);

	/*
	Getters, Setters & Checkers
	*/
	virtual void  setPolygon(QList<QPolygonF> polygons) = 0;
	virtual QList<QPolygonF> getPolygon() = 0;
	virtual float hasPolygon() = 0;

protected:
	QList<QPolygonF> _polygon;            /**< List of polygons, all will be drawn when vizualized */
};

/***************************************************************************//**
* This is a leaf of the Composite Pattern
* This class is derived from IModelComponentTemporal2D.
* It includes 2D and time data.
* This interface can be visualized in the core application if the tracking data implements it.
* An ellipse will be visualized.  The position is the center of the ellipse.
*******************************************************************************/
class IModelTrackedEllipse : public IModelTrackedPoint
{
	Q_OBJECT

public:
	/**
	* The constructor of the IModelTrackedEllipse class is able to receive a QObject as parent.
	*/
	IModelTrackedEllipse(QObject *parent = 0);

};

/***************************************************************************//**
* This is a leaf of the Composite Pattern
* This class is derived from IModelComponentTemporal2D.
* It includes 2D and time data.
* This interface can be visualized in the core application if the tracking data implements it.
* A rectangle will be visualized. The position is the center of the rectangle.
*******************************************************************************/
class IModelTrackedRectangle : public IModelTrackedPoint
{
	Q_OBJECT
	
public:
	/**
	* The constructor of the IModelTrackedRectangle class is able to receive a QObject as parent.
	*/
	IModelTrackedRectangle(QObject *parent = 0);
};

#endif // ITRACKEDCOMPONENT_H
