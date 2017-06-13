#ifndef TRACKERPARAMETER_H
#define TRACKERPARAMETER_H


#include "Interfaces/IModel/IModel.h"
#include "settings/Settings.h"
#include "Model/TrackingAlgorithm/property/ParamNames.h"

class TrackerParameter : public IModel
{
    Q_OBJECT
public:
    TrackerParameter(QObject *parent = 0);

    void setThreshold(int x);

    int getThreshold();

	BioTracker::Core::Settings *getSettings() { return _settings; };

	void setBinarizationThreshold(int x);
	int getBinarizationThreshold();

	int getSizeErode() { return m_SizeErode; };
	void setSizeErode(int x) {
		m_SizeErode = x;
		Q_EMIT notifyView();
	};

	int getSizeDilate() { return m_SizeDilate; };
	void setSizeDilate(int x) {
		m_SizeDilate = x;
		Q_EMIT notifyView();
	};

	int getmog2History() { return m_mog2History; };
	void setmog2History(int x) {
		m_mog2History = x;
		Q_EMIT notifyView();
	};

	int getmog2VarThresh() { return m_mog2VarThresh; };
	void setmog2VarThresh(int x) {
		m_mog2VarThresh = x;
		Q_EMIT notifyView();
	};

	double getmog2BackgroundRatio() { return m_mog2BackgroundRatio; };
	void setmog2BackgroundRatio(double x) {
		m_mog2BackgroundRatio = x;
		Q_EMIT notifyView();
	};

	double getMinBlobSize() { return m_MinBlobSize; };
	void setMinBlobSize(double x) {
		m_MinBlobSize = x;
		Q_EMIT notifyView();
	};

	double getMaxBlobSize() { return m_MaxBlobSize; };
	void setMaxBlobSize(double x) {
		m_MaxBlobSize = x;
		Q_EMIT notifyView();
	};

	bool getDoBackground() { return m_doBackground; };
	void setDoBackground(bool x) {
		m_doBackground = x;
		Q_EMIT notifyView();
	};

	bool getDoNetwork() { return m_doNetwork; };
	void setDoNetwork(bool x) {
		m_doNetwork = x;
		Q_EMIT notifyView();
	};

	int getSendImage() { return m_sendImage; };
	void setSendImage(int x) {
		m_sendImage = x;
		Q_EMIT notifyView();
	};

	bool getResetBackground() { return m_resetBackground; };
	void setResetBackground(bool x) {
		m_resetBackground = x;
		Q_EMIT notifyView();
	};

	int getNoFish() { return m_noFish; };
	void setNoFish(int x) {
		m_noFish = x;
		_settings->setParam(FISHTANKPARAM::FISHTANK_FISH_AMOUNT, x);
		Q_EMIT notifyView();
	};

	int getAreaWidth() { return m_areaWidth; };
	void setAreaWidth(int x) {
		m_areaWidth = x;
		_settings->setParam(FISHTANKPARAM::FISHTANK_AREA_WIDTH, x);
		Q_EMIT notifyView();
	};

	int getAreaHeight() { return m_areaHeight; };
	void setAreaHeight(int x) {
		m_areaHeight = x;
		_settings->setParam(FISHTANKPARAM::FISHTANK_AREA_HEIGHT, x);
		Q_EMIT notifyView();
	};


	

	void setAll(
		int Threshold,
		int BinarizationThreshold,
		int SizeErode,
		int SizeDilate,
		int mog2History,
		int mog2VarThresh,
		double mog2BackgroundRatio,
		int minBlobSize, 
		int maxBlobSize,
		int areaWidth,
		int areaHeight)
	{
		m_Threshold = Threshold;
		m_BinarizationThreshold = BinarizationThreshold;
		m_SizeErode = SizeErode;
		m_SizeDilate = SizeDilate;
		m_mog2History = mog2History;
		m_mog2VarThresh = mog2VarThresh;
		m_mog2BackgroundRatio = mog2BackgroundRatio;
		m_MinBlobSize = minBlobSize;
		m_MaxBlobSize = maxBlobSize;
		m_areaWidth = areaWidth;
		m_areaHeight = areaHeight;
		_settings->setParam(TRACKERPARAM::THRESHOLD_BINARIZING, BinarizationThreshold);
		_settings->setParam(TRACKERPARAM::SIZE_ERODE, SizeErode);
		_settings->setParam(TRACKERPARAM::SIZE_DILATE, SizeDilate);
		_settings->setParam(TRACKERPARAM::MIN_BLOB_SIZE, minBlobSize);
		_settings->setParam(TRACKERPARAM::MAX_BLOB_SIZE, maxBlobSize);
		_settings->setParam(TRACKERPARAM::BG_MOG2_HISTORY, mog2History);
		_settings->setParam(TRACKERPARAM::BG_MOG2_VAR_THRESHOLD, mog2VarThresh);
		_settings->setParam(TRACKERPARAM::BG_MOG2_BACKGROUND_RATIO, mog2BackgroundRatio);
		_settings->setParam(FISHTANKPARAM::FISHTANK_AREA_WIDTH, areaWidth);
		_settings->setParam(FISHTANKPARAM::FISHTANK_AREA_HEIGHT, areaHeight);
		Q_EMIT notifyView();
	};



private:
	BioTracker::Core::Settings *_settings;

	int m_Threshold;
	int m_BinarizationThreshold;
	int m_SizeErode;
	int m_SizeDilate;
	int m_mog2History;
	int m_mog2VarThresh;
	double m_mog2BackgroundRatio;
	int m_MinBlobSize;
	int m_MaxBlobSize;

	bool m_doBackground;
	int m_sendImage;
	bool m_resetBackground;
	int m_noFish;

	int m_areaWidth;
	int m_areaHeight;
	int m_networkPort;
	bool m_doNetwork;
};

#endif // TRACKERPARAMETER_H
