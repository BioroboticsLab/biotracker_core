#pragma once


#include <string>

class CameraConfiguration
{
public:
	CameraConfiguration() :
		_id(0), _width(-1), _height(-1), _fps(-1), _recordInput(false), _fourcc("FMP4") {
	}
	CameraConfiguration(int p_id, int p_width, int p_height, double p_fps, bool p_recordInput, std::string p_fourcc) :
		_id(p_id), _width(p_width), _height(p_height), _fps(p_fps), _recordInput(p_recordInput), _fourcc(p_fourcc) {
	}

	int _id;
	int _width;
	int _height;
	bool _recordInput;
	double _fps;
	std::string _fourcc;
};