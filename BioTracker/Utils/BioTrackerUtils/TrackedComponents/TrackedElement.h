#ifndef TRACKEDELEMENT_H
#define TRACKEDELEMENT_H

#include "QGraphicsItem"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include "QString"
#include <cv.h>
#include <qdatetime.h>
#include <ctime>

/**
* This class is an example of how a TrackedComponent could be defined.
* This class inherits from the IModelTrackedComponent class and is therefor part of the Composite Pattern.
* This class represents the Leaf class in the Composite Pattern.
* Objects of this class have a QObject as parent.
*/
class TrackedElement : public IModelTrackedPoint
{
	Q_OBJECT

public:
	TrackedElement(QObject *parent = 0, QString name = "n.a.", int id = 0);

	QString getName();
    QString getCoordinateUnit() override { return "px"; };

    void  setX(float val);
    void  setY(float val);
    void  setW(float w) {};
    void  setH(float h) {};
    void  setXpx(float val) {};
    void  setYpx(float val) {};
    void  setWpx(float w) {};
    void  setHpx(float h) {};
	void  setRad(float r) {};
	void  setDeg(float d) {};
	void  setId(int val) { _id = val; };
    void  setTime(std::chrono::system_clock::time_point t) {
        _timeSysclck = t;
    };
    void  setTime(qint64 t) {
        _time = t;
        std::string::size_type sz = 0;
        long long ll = t / 1000;
        std::time_t tm(ll);
        _timeSysclck = std::chrono::system_clock::from_time_t(tm);
    };
    void  setTimeString(QString t) { _timeString = t; };
	void  setValid(bool v) { _valid = v; };
	void  setFixed(bool f) { _fixed = f; };

    float getX();
    float getY();
    float getXpx();
    float getYpx();
    float getW() { return 0; };
    float getH() { return 0; };
    float getWpx() { return 0; };
    float getHpx() { return 0; };
	float getRad() { return 0; };
	float getDeg() { return 0; };
	int   getId() { return _id; };
    qint64  getTime() {
        qint64 q(std::chrono::duration_cast<std::chrono::milliseconds>(_timeSysclck.time_since_epoch()).count());
        return q;
    };
    QString getTimeString() {
        std::time_t t = std::chrono::system_clock::to_time_t(_timeSysclck);
        QDateTime dt;
        dt.setTime_t(t);
        _timeString = dt.toString();
        return _timeString;
    };
	bool  getValid() { return _valid; };
	bool  getFixed() { return _fixed; };

	bool hasX() { return true; };
	bool hasY() { return true; };
	bool hasW() { return false; };
	bool hasH() { return false; };
    bool hasXpx() { return false; };
    bool hasYpx() { return false; };
    bool hasWpx() { return false; };
    bool hasHpx() { return false; };
	bool hasRad() { return false; };
	bool hasDeg() { return false; };
	bool hasTime() { return true; };
    bool hasTimeString() { return true; };

	void setPoint(cv::Point2f p);
    cv::Point2f getPoint();

	// ITrackedPoint interface
public:
	void operate();

private:
    std::chrono::system_clock::time_point _timeSysclck;
	QString _name;
	float _x;
	float _y;
	int _id;
	bool _valid;
	bool _fixed;
};

#endif // TRACKEDELEMENT_H
