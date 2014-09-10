#include "Ellipse.h"

using namespace cv;
using namespace std;

namespace decoder {

Ellipse::Ellipse() {
	vote = 0;
	angle = 0;
}

Ellipse::Ellipse( int vote, Point2i center, Size axis_length, double angle ){
	this->vote = vote;
	this->angle = angle;
	cen = center;
	axis = axis_length;
}

double Ellipse::getAngle() const {
	return (angle);
}

void Ellipse::setAngle(double angle) {
	this->angle = angle;
}

Size Ellipse::getAxis() const {
	return (axis);
}

void Ellipse::setAxis(Size axis) {
	this->axis = axis;
}

Point2i Ellipse::getCen() const {
	return (cen);
}

void Ellipse::setCen(Point2i cen) {
	this->cen = cen;
}

 int Ellipse::getVote() const {
	return (vote);
}

void Ellipse::setVote( int vote) {
	this->vote = vote;
}

bool Ellipse::operator<(const Ellipse & elli2) const {
	return (vote < elli2.vote);
}

}
