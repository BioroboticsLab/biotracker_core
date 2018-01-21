#ifndef ITRACKEDCOMPONENT_H
#define ITRACKEDCOMPONENT_H

#include "IModel.h"
#include <chrono>

#include <QObject>
#include <QDataStream>
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
	Q_PROPERTY(bool valid READ getValid() WRITE setValid STORED true)
	Q_PROPERTY(double id READ getId() WRITE setId STORED true)
public:
	/**
	* The constructor of the IModelTrackedComponent class is able to receive a QObject as parent.
	*/
	IModelTrackedComponent(QObject *parent = 0);
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
	int _id;
	bool _valid;
	bool _fixed;
    IModelTrackedComponent *_parentNode = nullptr;
};

QDataStream &operator<<(QDataStream &out, const IModelTrackedComponent &painting);
QDataStream &operator>>(QDataStream &in, IModelTrackedComponent &painting);

class IModelComponentEuclidian2D : public IModelTrackedComponent {
public:
	Q_OBJECT
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
	IModelComponentEuclidian2D(QObject *parent = 0);

    /* Returns a human-readable string about coordinate units.
    * Freely enter something like "px", "cm" or "um". This will be annotated in the output.
    */
	virtual QString getCoordinateUnit() { return "cm"; };
	virtual void setCoordinateUnit(QString c) { _coordinateUnit = c; };

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
	QString _coordinateUnit;
	float _x;
	float _y;
	float _w;
	float _h;
	float _rad;
	float _deg;
	float _xpx;
	float _ypx;
	float _wpx;
	float _hpx;
};

class IModelComponentEuclidian3D : public IModelComponentEuclidian2D {
public:
	Q_OBJECT
	Q_PROPERTY(float z     READ getZ()			WRITE setZ          STORED hasZ)
	Q_PROPERTY(float zpx   READ getZpx()		WRITE setZpx        STORED hasZpx)
	Q_PROPERTY(float d     READ getD()			WRITE setD          STORED hasD)
	Q_PROPERTY(float dpx   READ getDpx()	    WRITE setDpx        STORED hasDpx)
	Q_PROPERTY(float rada2 READ getRadAxis2()  WRITE setRadAxis2   STORED hasRadAxis2)
	Q_PROPERTY(float rada3 READ getRadAxis3()  WRITE setRadAxis3   STORED hasRadAxis3)

public:
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
	float _z;
	float _zpx;
	float _d;
	float _dpx;
	float _rada2;
	float _rada3;
};

class IModelComponentTemporal {
public:
    virtual void  setTime(std::chrono::system_clock::time_point t) = 0;
    virtual std::chrono::system_clock::time_point  getTime() = 0;

protected:
	std::chrono::system_clock::time_point _time;
};

/*
*	Point
*/
class IModelTrackedPoint : public IModelComponentEuclidian2D, 
    public IModelComponentTemporal
{
	Q_OBJECT
	//Q_PROPERTY(double _time READ getTime() WRITE setTime)

public:
	/**
	* The constructor of the IModelTrackedPoint class is able to receive a QObject as parent.
	*/
	IModelTrackedPoint(QObject *parent = 0);
};

/**
*	Polygon
*/

class IModelTrackedPolygon : public IModelComponentEuclidian2D,
    public IModelComponentTemporal
{
	Q_OBJECT

public:

	/**
	* The constructor of the IModelTrackedPoint class is able to receive a QObject as parent.
	*/
	IModelTrackedPolygon(QObject *parent = 0);

	// TODO: check if overlapping needed?
	// List of Polygons; first one is the outer Polygon the others are inner Polygons (holes)
	virtual void  setPolygon(QList<QList<QPoint>> polygons) = 0;

	// Get list of Polygons; first one is the outer Polygon the others are inner Polygons (holes)
	virtual QList<QList<QPoint>> getPolygon() = 0;

	// Check if list of Polygons exists; first one is the outer Polygon the others are inner Polygons (holes)
	virtual float hasPolygon() = 0;
};

class IModelTrackedEllipse : public IModelTrackedPoint
{
	Q_OBJECT

public:
	/**
	* The constructor of the IModelTrackedEllipse class is able to receive a QObject as parent.
	*/
	IModelTrackedEllipse(QObject *parent = 0);

};

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
