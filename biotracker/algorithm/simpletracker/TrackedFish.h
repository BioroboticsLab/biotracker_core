#ifndef TrackedFish_H
#define TrackedFish_H

#include "biotracker/tracking/serialization/TrackedObject.h"

#include <cereal/access.hpp>
#include <opencv2/opencv.hpp>

class TrackedFish :
    public ObjectModel {
  public:
    TrackedFish() {}
    virtual ~TrackedFish() override {}

    void setNextPosition(cv::Point2f position);
    void setNextPositionUnknown();

    cv::Point2f last_known_position() const;
    unsigned age_of_last_known_position() const;

    void set_associated_color(const cv::Scalar &color);
    cv::Scalar associated_color() const;

  protected:
    cv::Point2f _last_known_position;
    unsigned    _age_of_last_known_position;
    cv::Scalar  _associated_color;

  private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &ar) {
        ar(CEREAL_NVP(_last_known_position),
           CEREAL_NVP(_age_of_last_known_position),
           CEREAL_NVP(_associated_color));
    }
};

#endif
