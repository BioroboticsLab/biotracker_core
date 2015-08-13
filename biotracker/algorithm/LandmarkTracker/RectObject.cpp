#include "RectObject.h"

#include <biotracker/tracking/serialization/types.hpp>

#include <cereal/archives/json.hpp>
#include <cereal/types/polymorphic.hpp>

RectObject::RectObject()
    : RectObject(size_t(0), cv::Point(0, 0), cv::Point(0, 0)) {
}

RectObject::RectObject(size_t frameNr, cv::Point rectStart, cv::Point rectEnd)
    : _frameNr(frameNr)
    , _rectStart(rectStart)
    , _rectEnd(rectEnd) {
    //prepare_visualization_data();
}

RectObject::~RectObject() = default;

size_t RectObject::getRectFrameNr() const {
    return _frameNr;
}

cv::Point RectObject::getRectStart() const {
    return _rectStart;
}

cv::Point RectObject::getRectEnd() const {
    return _rectEnd;
}

CEREAL_REGISTER_TYPE(RectObject)
