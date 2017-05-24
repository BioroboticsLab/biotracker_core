#include "TrackerParameter.h"

TrackerParameter::TrackerParameter(QObject *parent) :
    IModel(parent)
{
	m_Threshold = 12345;
	m_BinarizationThreshold = 40;
	m_SizeErode = 8;
	m_SizeDilate = 5;
	m_mog2History = 200;
	m_mog2VarThresh = 64;
	m_mog2BackgroundRatio = 0.05;
	m_MinBlobSize = 40;
	m_MaxBlobSize = 999999;
	m_doBackground = true;
	m_doNetwork = false;
	m_sendImage = 0; //Send no image
	m_resetBackground = false;
	m_noFish = 2;

    Q_EMIT notifyView();
}

void TrackerParameter::setThreshold(int x)
{
    m_Threshold = x;

    Q_EMIT notifyView();
}

int TrackerParameter::getThreshold()
{
    return m_Threshold;
}

void TrackerParameter::setBinarizationThreshold(int x)
{
	m_BinarizationThreshold = x;

	Q_EMIT notifyView();
}

int TrackerParameter::getBinarizationThreshold()
{
	return m_BinarizationThreshold;
}