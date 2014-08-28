#ifndef TrackedFish_H
#define TrackedFish_H

#include "source/trackedObject/TrackedObject.h"
#include "source/trackedObject/History.h"

#include <opencv.hpp>

class TrackedFish :
	public TrackedObject
{
public:
	TrackedFish(int id, History his);
	virtual ~TrackedFish(void);
	int id() const;
	void setNextPosition(cv::Point2f position);
	void setNextPositionUnknown();
	cv::Point2f last_known_position() const;
	unsigned age_of_last_known_position() const;
	void set_associated_color(const cv::Scalar& color);
	cv::Scalar associated_color();
protected:
	cv::Point2f _last_known_position;
	unsigned _age_of_last_known_position;
	cv::Scalar _associated_color;
};

#endif