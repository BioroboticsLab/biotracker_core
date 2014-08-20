#include "TrackedFish.h"


TrackedFish::TrackedFish(int id, History his) : TrackedObject(id, his)
{
}


TrackedFish::~TrackedFish(void)
{
}

void TrackedFish::setNextPosition(cv::Point2f position) {
	_age_of_last_known_position = 1;
	_last_known_position = position;
}

void TrackedFish::setNextPositionUnknown() {
	++_age_of_last_known_position;
}

cv::Point2f TrackedFish::last_known_position() const {
	return _last_known_position;
}

unsigned TrackedFish::age_of_last_known_position() const {
	return _age_of_last_known_position;
}

