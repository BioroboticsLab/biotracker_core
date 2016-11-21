/*
 * CvHelper.cpp
 *
 *  Created on: May 12, 2015
 *      Author: tobias
 */

#include "util/CvHelper.h"

namespace BioTracker {
namespace Core {
namespace CvHelper {

std::string cvDataType2str(int type) {
    const int depth = CV_MAT_DEPTH(type);
    const int cn = CV_MAT_CN(type);
    std::string result = "CV_";

    switch (depth) {
    case CV_8U:
        result += "8U";
        break;
    case CV_8S:
        result += "8S";
        break;
    case CV_16U:
        result += "16U";
        break;
    case CV_16S:
        result += "16S";
        break;
    case CV_32S:
        result += "32S";
        break;
    case CV_32F:
        result += "32F";
        break;
    case CV_64F:
        result += "64F";
        break;
    default:
        result += "UNKOWN";
        break;
    }
    result += "C" + std::to_string(cn);
    return result;
}

}
}
}
