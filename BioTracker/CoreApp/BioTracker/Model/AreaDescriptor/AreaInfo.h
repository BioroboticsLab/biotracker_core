#pragma once
#include "Interfaces/IModel/IModelAreaDescriptor.h"
#include "Interfaces/IModel/IModel.h"
#include "AreaInfoElement.h"
#include "util/types.h"
#include "Model/MediaPlayerStateMachine/PlayerParameters.h"

class AreaInfo : public IModelAreaDescriptor
{
	Q_OBJECT
public:
	AreaInfo() = delete;
	AreaInfo(QObject *parent = 0);

	//TODO PORT
	bool inTrackingArea(cv::Point2f point_cm) override; 

	/**
	* Transform the provided pixel coordinates into world coordinates and return world coordinates.
	* @param: pixelCoords, a list of points.
	* @return: world coordinates in as a list.
	*/
	cv::Point2f pxToCm(cv::Point point_px) override;

	/**
	* Transform the provided pixel point into world coordinates and return world point.
	* @param: point, a pixel point, used opencv point
	* @return: world point.
	*/
	cv::Point2f cmToPx(cv::Point2f point_cm) override;

	void updateApperture();

	void updateRectification();

	void setRectificationDimensions(double w, double h);

    void reset(int w, int h);
    void loadAreas();
    void setUseEntireScreen(bool b) { _useEntireScreen = b; };
    bool getUseEntireScreen() { return _useEntireScreen; };

	std::shared_ptr<AreaInfoElement> _rect;
	std::shared_ptr<AreaInfoElement> _apperture;

private:

    //If the user never changed the area, use the whole screen. Indicate by this switch
    bool _useEntireScreen = false;
    int _vdimX = 1;
    int _vdimY = 1;
    playerParameters* _parms;
    bool _rectInitialized = false;

public Q_SLOTS:
    void rcvPlayerParameters(playerParameters* parameters);

private:
    QString myType();
};

