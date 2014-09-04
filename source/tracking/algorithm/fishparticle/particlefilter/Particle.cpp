#include "Particle.h"
#include <QtCore/qmath.h>

Particle::Particle(void)
{
}

Particle::Particle(int x, int y, float a, int id) :
	_x(x), 
	_y(y), 
	_angle_degree(a),
	_id(id)
{}

Particle::~Particle(void)
{
}

void Particle::add(Particle p)
{
	_x				+= p.getX();
	_y				+= p.getY();
	_angle_degree	+= p.getAngleDegree();
	//_angle_degree	= angle_add( _angle_degree, p.getAngleDegree() );
}

void Particle::mul(double a)
{
	_x				*= a;
	_y				*= a;
	_angle_degree	*= a;
}

void Particle::mean(Particle p)
{
	_x				= ( _x + p.getX() ) / 2; 
	_y				= ( _y + p.getY() ) / 2; 
	_angle_degree	= mean_angle(_angle_degree, p.getAngleDegree() );
}

void Particle::mean_weighted(Particle p, float w1, float w2)
{
	if (w1 == 0 && w2 ==0)
	{
		w1 =  _belief;
		w2 = p.getBelief();

		double sum = w1 + w2 ;
		w1		= w1 / sum;
		w2		= w2 / sum;
	}

	_x				= ( w1 * _x + w2* p.getX() ) / ( w1 + w2 ); 
	_y				= ( w1 * _y + w2* p.getY() ) / ( w1 + w2 ); 
	_angle_degree	= mean_angle_weighted(_angle_degree, p.getAngleDegree(), w1, w2 );
	_belief			= (w1*_belief + w2*p.getBelief() ) / ( w1 + w2 );
}

double Particle::mean_angle(double a, double b)
{
	return 180 * qAtan2( 0.5*(sin(b) + sin(a)), 0.5*(cos(b) + cos(a)) ) / M_PI;
}

double Particle::mean_angle_weighted( double a, double b, float w1, float w2)
{
		return 180 * qAtan2( (w1*sin(b) + w2*sin(a))/(w1+w2), (w1*cos(b) + w2*cos(a))/(w1+w2) ) / M_PI;
}

double Particle::dist(Particle p)
{
	/*return	qSqrt(	qPow( _x - p.getX(), 2 ) + 
					qPow( _y - p.getY(), 2 ) +
					qPow( CvHelper::radToDeg(CvHelper::getAngleDifference( CV_PI * _angle_degree / 180.0, CV_PI * p.getAngleDegree() / 180.0 )) ,2 ) );*/
	return	qSqrt(	qPow( _x - p.getX(), 2 ) + 
					qPow( _y - p.getY(), 2 ) );
}