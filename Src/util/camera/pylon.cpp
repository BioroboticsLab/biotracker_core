#include "util/camera/pylon.h"

#if HAS_PYLON

Pylon::IPylonDevice* getPylonDevice(int index)
{
	Pylon::PylonAutoInitTerm pylon;
	auto& factory = Pylon::CTlFactory::GetInstance();
	Pylon::DeviceInfoList_t devices;
	factory.EnumerateDevices(devices);
	if (0 <= index && index < devices.size())
		return factory.CreateDevice(devices[index]);
	throw std::out_of_range("No such Pylon camera available");
}

cv::Mat toOpenCV(Pylon::CGrabResultPtr image)
{
	return {
		cv::Size{static_cast<int>(image->GetWidth()), static_cast<int>(image->GetHeight())},
		[](Pylon::EPixelType type){
			switch (type) {
			case Pylon::PixelType_Mono8:
				return CV_8UC1;
			default:
				throw std::logic_error("Pixel type support not implemented");
			}
		}(image->GetPixelType()),
		image->GetBuffer()
	};
}
#endif
