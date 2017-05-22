#ifndef TRACKERPARAMETER_H
#define TRACKERPARAMETER_H


#include "Interfaces/IModel/IModel.h"

class TrackerParameter : public IModel
{
    Q_OBJECT
public:
    TrackerParameter(QObject *parent = 0);

    void setThreshold(int x);

    int getThreshold();


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


	void setAll(
		int Threshold,
		int BinarizationThreshold,
		int SizeErode,
		int SizeDilate,
		int mog2History,
		int mog2VarThresh,
		double mog2BackgroundRatio,
		int minBlobSize, 
		int maxBlobSize)
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
		Q_EMIT notifyView();
	};



private:
	int m_Threshold;
	int m_BinarizationThreshold;
	int m_SizeErode;
	int m_SizeDilate;
	int m_mog2History;
	int m_mog2VarThresh;
	double m_mog2BackgroundRatio;
	int m_MinBlobSize;
	int m_MaxBlobSize;
};

#endif // TRACKERPARAMETER_H
