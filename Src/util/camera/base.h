#pragma once

#include <string>

#include <QMetaType>

enum class CameraType
{
	OpenCV,
#if HAS_PYLON
	Pylon,
#endif
};

struct CameraSelector
{
	CameraType type;
	int        index;
};

Q_DECLARE_METATYPE(CameraSelector);

class CameraConfiguration
{
public:
	CameraConfiguration() :
		_selector{CameraType::OpenCV, 0}, _width(-1), _height(-1), _fps(30), _recordInput(false), _fourcc("X264") {
	}
	CameraConfiguration(CameraSelector p_selector, int p_width, int p_height, double p_fps, bool p_recordInput, std::string p_fourcc) :
		_selector(p_selector), _width(p_width), _height(p_height), _fps(p_fps), _recordInput(p_recordInput), _fourcc(p_fourcc) {
	}

	CameraSelector _selector;
	int _width;
	int _height;
	bool _recordInput;
	double _fps;
	std::string _fourcc;
};
