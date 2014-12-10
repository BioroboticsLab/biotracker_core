#pragma once

#include <cereal/cereal.hpp>
#include <cereal/access.hpp>

#include <opencv2/opencv.hpp>
#include <boost/logic/tribool.hpp>

namespace cereal {
template<class Archive>
void serialize(Archive& archive, cv::Point2f& point)
{
	archive(CEREAL_NVP(point.x), CEREAL_NVP(point.y));
}

template<class Archive>
void serialize(Archive& archive, cv::Size2f& size)
{
	archive(CEREAL_NVP(size.width), CEREAL_NVP(size.height));
}

template<class Archive>
void serialize(Archive& archive, boost::logic::tribool& tribool)
{
	archive(CEREAL_NVP(tribool.value));
}

template<class Archive>
void serialize(Archive& archive, cv::Scalar& scalar)
{
	archive(CEREAL_NVP(scalar.val));
}
}
