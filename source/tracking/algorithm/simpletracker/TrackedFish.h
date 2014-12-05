#ifndef TrackedFish_H
#define TrackedFish_H

#include "source/tracking/trackedObject/TrackedObject.h"

#include <cereal/access.hpp>
#include <opencv2/opencv.hpp>

namespace cereal {
template<class Archive>
void serialize(Archive& archive, cv::Point2f& point)
{
	archive(CEREAL_NVP(point.x), CEREAL_NVP(point.y));
}

template<class Archive>
void serialize(Archive& archive, cv::Scalar& scalar)
{
	archive(CEREAL_NVP(scalar.val));
}
}

class TrackedFish :
    public ObjectModel
{
public:
    TrackedFish() {}
    virtual ~TrackedFish() override {}

    void setNextPosition(cv::Point2f position);
    void setNextPositionUnknown();

    cv::Point2f last_known_position() const;
    unsigned age_of_last_known_position() const;

    void set_associated_color(const cv::Scalar& color);
    cv::Scalar associated_color() const;

protected:
    cv::Point2f _last_known_position;
    unsigned    _age_of_last_known_position;
    cv::Scalar  _associated_color;

private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& ar)
    {
		ar(CEREAL_NVP(_last_known_position),
		   CEREAL_NVP(_age_of_last_known_position),
		   CEREAL_NVP(_associated_color));
    }
};

#endif
