#include "Particle.h"
#include <QtCore/qmath.h>

/**
* Constructs a particle with x, y positions, angle and an ID. The ID should be unique within a set of particles for a frame.
*/
Particle::Particle(int x, int y, float a, int id) :
    _x(x),
    _y(y),
    _angle_degree(a),
    _score(0),
    _id(id) {
}

Particle::~Particle(void) {
}

/**
* Calculates the euclidian distance between 2 particles. Always >= 0.
*/
double Particle::dist(const Particle &p) const {
    /*return    qSqrt(  qPow( _x - p.getX(), 2 ) +
    qPow( _y - p.getY(), 2 ) +
    qPow( CvHelper::radToDeg(CvHelper::getAngleDifference( CV_PI * _angle_degree / 180.0, CV_PI * p.getAngleDegree() / 180.0 )) ,2 ) );*/
    return  qSqrt(qPow(_x - p.getX(), 2) +
                  qPow(_y - p.getY(), 2));
}
