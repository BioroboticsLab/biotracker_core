#pragma once

#if HAS_PYLON

#include "util/camera/base.h"

#include <opencv2/core/mat.hpp>
#include <pylon/PylonIncludes.h>
#include <pylon/usb/BaslerUSBInstantCamera.h>

Pylon::IPylonDevice* getPylonDevice(int index);
cv::Mat toOpenCV(Pylon::CGrabResultPtr image);

#endif
