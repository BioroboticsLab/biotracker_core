#include "TrackerParameter.h"

TrackerParameter::TrackerParameter(QObject *parent) :
    IModel(parent)
{
	m_Threshold = 12345;
	m_BinarizationThreshold = 25;
	m_SizeErode = 3;
	m_SizeDilate = 2;
	m_mog2History = 200;
	m_mog2VarThresh = 64;
	m_mog2BackgroundRatio = 0.05;
	m_MinBlobSize = 5;
	m_MaxBlobSize = 0.05;

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