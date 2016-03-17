#pragma once

#include <opencv2/opencv.hpp> // cv::Point, cv::Rect
#include <QRect>              // QRect
#include <QPoint>             // QPoint
#include <string>             // std::string; std::to_string(int)

/**
 * Computer vision helper functions
 */
namespace BioTracker {
namespace Core {
namespace CvHelper {
struct cv_point_compare_less_x {
    template<typename CV_POINT>
    bool operator()(const CV_POINT &lhs, const CV_POINT &rhs) const {
        return lhs.x < rhs.x;
    }
};

struct cv_point_compare_less_y {
    template<typename CV_POINT>
    bool operator()(const CV_POINT &lhs, const CV_POINT &rhs) const {
        return lhs.y < rhs.y;
    }
};

inline cv::Rect toCv(const QRect &rect) {
    return cv::Rect(rect.x(), rect.y(), rect.width(), rect.height());
}

inline QRect toQt(const cv::Rect &rect) {
    return QRect(rect.x,rect.y,rect.width,rect.height);
}

inline QPoint toQt(const cv::Point &point) {
    return QPoint(point.x, point.y);
}

/**
 * @see: http://en.cppreference.com/w/cpp/algorithm/lexicographical_compare
 */
struct cv_vec_compare_less_lexicographically {
    template<typename CV_VEC>
    bool operator()(const CV_VEC &lhs, const CV_VEC &rhs) const {
        for (int i = 0; i < CV_VEC::channels; ++i) {
            if (lhs[i] < rhs[i]) {
                return true;
            }
            if (rhs[i] < lhs[i]) {
                return false;
            }
        }
        return false;
    }
};

/**
 * converts an opencv data type value (int) to the name of the corresponding macro i.e. "CV_\d+[UFS]C\d"
 *
 * @see: cv::DataType<T>::type
 *
 * @example:
 *   CvHelper::cvDataType2str( CV_8UC3 ) --> std::string( "CV_8UC3" )
 *
 */
std::string cvDataType2str(int type);

}
}
}
