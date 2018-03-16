#include "TrackedElement.h"
#include "TrackedTrajectory.h"
#include "QDebug"
#include "QRect"
#include "QBrush"
#include "QPainter"
#include "QtMath"

TrackedElement::TrackedElement(QObject *parent, QString name, int id) :
	IModelTrackedPoint(parent)
{
	_x = 0;
	_y = 0;
    _id = id;
	_deg = 0;
	_rad = 0;
	_valid = false;
	_fixed = false;
}

void  TrackedElement::setValid(bool v) 
{ 
    _valid = v;
    if (_parentNode) {
        TrackedTrajectory* n = dynamic_cast<TrackedTrajectory*>(_parentNode);
        if (n)
            n->triggerRecalcValid();
    }
}


QString TrackedElement::getName()
{
	return "";
}

void TrackedElement::setFishPose(FishPose p)
{
	_pose = p;
	_x = p.position_cm().x;
	_y = p.position_cm().y;
	_deg = p.orientation_deg();
	_rad = p.orientation_rad();
    _ypx = p.position_px().y;
    _xpx = p.position_px().x;
	_valid = true;
	Q_EMIT notifyView();
}

void TrackedElement::setTime(std::chrono::system_clock::time_point t) {
    _timeSysclck = t;
};

void TrackedElement::setTime(qint64 t) {
    _time = t;
    std::string::size_type sz = 0;
    long long ll = t / 1000;
    long long remainder = t % 1000;
    std::time_t tm(ll);

    _timeSysclck = std::chrono::system_clock::from_time_t(tm); 
    std::chrono::duration<long long, std::milli> dur(remainder);
    _timeSysclck += dur;
};

qint64 TrackedElement::getTime() {
    qint64 q(std::chrono::duration_cast<std::chrono::milliseconds>(_timeSysclck.time_since_epoch()).count());
    return q;
};

QString TrackedElement::getTimeString() {
    std::time_t t = std::chrono::system_clock::to_time_t(_timeSysclck);
    QDateTime dt;
    dt.setTime_t(t);
    _timeString = dt.toString();
    return _timeString;
};

FishPose TrackedElement::getFishPose()
{
	return _pose;
}

void  TrackedElement::setX(float val) {
	_x = val;
	FishPose pnew(cv::Point2f(_x, _pose.position_cm().y), cv::Point2f(-1, -1), _rad, _deg, _pose.width(), _pose.height(), _pose.getScore());
    _ypx = _pose.position_px().y;
    _xpx = _pose.position_px().x;
	_pose = pnew;
};

void  TrackedElement::setY(float val) {
	_y = val;
	FishPose pnew(cv::Point2f(_pose.position_cm().x, _y), cv::Point2f(-1, -1), _rad, _deg, _pose.width(), _pose.height(), _pose.getScore());
    _ypx = _pose.position_px().y;
    _xpx = _pose.position_px().x;
	_pose = pnew;
};

void  TrackedElement::setRad(float r) {
	_rad = r;
	FishPose pnew(_pose.position_cm(), _pose.position_px(), _rad, _deg, _pose.width(), _pose.height(), _pose.getScore());
    _pose = pnew;
};

void  TrackedElement::setDeg(float d) {
	_deg = d;
	FishPose pnew(_pose.position_cm(), _pose.position_px(), _rad, _deg, _pose.width(), _pose.height(), _pose.getScore());
    _pose = pnew;

};

void  TrackedElement::setW(float w) {
	_w = w;
	FishPose pnew(_pose.position_cm(), _pose.position_px(), _rad, _deg, _w, _pose.height(), _pose.getScore());
    _pose = pnew;
}

void TrackedElement::setH(float h) {
	_h = h;
	FishPose pnew(_pose.position_cm(), _pose.position_px(), _rad, _deg, _pose.width(), _h, _pose.getScore());
    _pose = pnew;
}


void TrackedElement::setXpx(float val) {
    _xpx = val;
}

void TrackedElement::setYpx(float val) {
    _ypx = val;
}

void TrackedElement::setWpx(float w) {
	std::cout << "Stub..." << std::endl;
}

void TrackedElement::setHpx(float h) {
	std::cout << "Stub..." << std::endl;
}

float TrackedElement::getX() {
	return _pose.position_cm().x;
}

float TrackedElement::getY() {
	return _pose.position_cm().y;
}

float TrackedElement::getXpx() {
    return _xpx;// _pose.position_px().x;
}

float TrackedElement::getYpx() {
    return _ypx;// _pose.position_px().y;
}

float TrackedElement::getDeg() {
	return _pose.orientation_deg();
}

void TrackedElement::operate()
{
	qDebug() << "I am TrackedElement ";
}

void TrackedElement::pressed()
{
	Q_EMIT notifyView();

}

void TrackedElement::notPressed()
{
	Q_EMIT notifyView();
}

bool TrackedElement::getPressedStatus()
{
	return false;
}



