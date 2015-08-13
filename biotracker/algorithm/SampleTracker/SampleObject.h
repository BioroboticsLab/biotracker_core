#pragma once

#include <opencv2/opencv.hpp>

#include "biotracker/core/serialization/ObjectModel.h"

class SampleObject : public ObjectModel {
  public:
    SampleObject(void);
    virtual ~SampleObject(void);

    void setPosition(const cv::Point pos);
    cv::Point getPosition();

  private:
    cv::Point _position;
};
