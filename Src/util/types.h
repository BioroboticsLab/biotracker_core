#pragma once


#include <string>
#include <vector>

namespace BiotrackerTypes{
    enum AreaType {
        NONE = -1,
        RECT = 0,
        APPERTURE = 1,
        COMBINED = 2
    };

}

namespace AREADESCRIPTOR
{
	const std::string CN_ARENA = "TRACKERPARAM/CN_CORNERS_ARENA";
	const std::string CN_APPERTURE = "TRACKERPARAM/CN_CORNERS_APPERTURE";
	const std::string CN_APPERTURE_TYPE = "TRACKERPARAM/CN_CORNERS_APPERTURE_TYPE";
    const std::string DISP_AREA = "TRACKERPARAM/DISPLAY_TRACKING_AREA";
    const std::string DISP_RECT = "TRACKERPARAM/DISPLAY_RECTIFICATION_AREA";
    const std::string RECT_W = "RECTIFICATION/WIDTH";
    const int RECT_W_DEFAULT = 100;
	const std::string RECT_H = "RECTIFICATION/HEIGHT";
    const int RECT_H_DEFAULT = 100;
}


const std::vector<std::pair<std::string, std::string>> codecList = {
	std::pair<std::string, std::string>("X264 (CPU)", "X264"),
#ifdef WITH_CUDA
	std::pair<std::string, std::string>("X264 (GPU)", "X264GPU")
#endif
};

const std::vector<std::string> exporterList = {
	std::string("CSV"),
    std::string("Serialize"),
    std::string("Json")
};
