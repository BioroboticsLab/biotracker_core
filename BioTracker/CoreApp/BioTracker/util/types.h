#pragma once


#include <string>

#ifndef CORE_CONFIGURATION
#define CORE_CONFIGURATION					"BiotrackerCore.ini"
#define CFG_CODEC							"BiotrackerCore/CodecUsed"
#define CFG_DROPFRAMES						"BiotrackerCore/DropFrames"
#define CFG_DROPFRAMES_VAL					false
#define CFG_RECORDSCALEDOUT					"BiotrackerCore/RecordScaledOutput"
#define CFG_EXPORTER						"BiotrackerCore/DataExporter"
#define CFG_RECORD_FPS						"BiotrackerCore/Record_FPS"
#define CFG_RECORD_FPS_VAL					30
#define CFG_CAMERA_DEFAULT_W				"BiotrackerCore/CameraWidth"
#define CFG_CAMERA_DEFAULT_W_VAL			-1
#define CFG_CAMERA_DEFAULT_H				"BiotrackerCore/CameraHeight"
#define CFG_CAMERA_DEFAULT_H_VAL			-1
#define CFG_INPUT_FRAME_STRIDE				"BiotrackerCore/FrameStride"
#define CFG_INPUT_FRAME_STRIDE_VAL			1
#endif



const std::vector<std::pair<std::string, std::string>> codecList = {
	std::pair<std::string, std::string>("X264 (CPU)", "X264"),
#ifdef WITH_CUDA
	std::pair<std::string, std::string>("X264 (GPU)", "X264GPU")
#endif
};

const std::vector<std::string> exporterList = {
	std::string("CSV")
};

class CameraConfiguration
{
public:
	CameraConfiguration() :
		_id(0), _width(CFG_CAMERA_DEFAULT_W_VAL), _height(CFG_CAMERA_DEFAULT_H_VAL), _fps(CFG_RECORD_FPS_VAL), _recordInput(false), _fourcc("X264") {
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



