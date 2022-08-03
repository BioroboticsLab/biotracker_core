#include "util/camera/pylon.h"

#if HAS_PYLON

    #include <opencv2/imgproc.hpp>

Pylon::IPylonDevice* getPylonDevice(int index)
{
    Pylon::PylonAutoInitTerm pylon;
    auto&                    factory = Pylon::CTlFactory::GetInstance();
    Pylon::DeviceInfoList_t  devices;
    factory.EnumerateDevices(devices);
    if (0 <= index && index < devices.size())
        return factory.CreateDevice(devices[index]);
    throw std::out_of_range("No such Pylon camera available");
}

cv::Mat toOpenCV(Pylon::CGrabResultPtr image)
{
    auto size = cv::Size{static_cast<int>(image->GetWidth()),
                         static_cast<int>(image->GetHeight())};
    auto data = image->GetBuffer();

    switch (image->GetPixelType()) {
    case Pylon::PixelType_Mono8: {
        auto mat = cv::Mat{size, CV_8UC1, data};
        cv::cvtColor(mat, mat, cv::COLOR_GRAY2BGR);
        return mat;
    }
    case Pylon::PixelType_BayerBG8: {
        auto mat = cv::Mat{size, CV_8UC1, data};
        cv::cvtColor(mat, mat, cv::COLOR_BayerBG2BGR_EA);
        return mat;
    }
    default:
        throw std::logic_error("Pixel type support not implemented");
    }
}
#endif
