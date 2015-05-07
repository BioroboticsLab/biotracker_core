#pragma once

#include <opencv2/opencv.hpp> // CV_PI, cv::Matx
#include <cmath>              // std::{sin,cos}
#include <stdexcept>          // std::invalid_argument
#include <QRect>

/**
 * Computer vision helper functions
 */
namespace CvHelper
{
	struct cv_point_compare_less_x
	{
		template<typename CV_POINT>
		bool operator()(const CV_POINT &lhs, const CV_POINT &rhs) const {
			return lhs.x < rhs.x;
		}
	};

	struct cv_point_compare_less_y
	{
		template<typename CV_POINT>
		bool operator()(const CV_POINT &lhs, const CV_POINT &rhs) const {
			return lhs.y < rhs.y;
		}
	};

	inline cv::Rect toCv(const QRect& rect)
	{
		return cv::Rect( rect.x(), rect.y(), rect.width(), rect.height() );
	}

	inline QRect toQt(const cv::Rect& rect)
	{
		return QRect(rect.x,rect.y,rect.width,rect.height);
	}

	inline QPoint toQt( const cv::Point& point )
	{
		return QPoint( point.x, point.y );
	}

	/**
	 * @see: http://en.cppreference.com/w/cpp/algorithm/lexicographical_compare
	 */
	struct cv_vec_compare_less_lexicographically
	{
		template<typename CV_VEC>
		bool operator()(const CV_VEC &lhs, const CV_VEC &rhs) const {
			for (size_t i = 0; i < CV_VEC::channels; ++i) {
				if (lhs[i] < rhs[i]) { return true;  }
				if (rhs[i] < lhs[i]) { return false; }
			}
			return false;
		}
	};

}
