#include "TrackerParameter.h"

TrackerParameter::TrackerParameter(QObject *parent) :
    IModel(parent)
{
    m_Threshold = 12345;

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
