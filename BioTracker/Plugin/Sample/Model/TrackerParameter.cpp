#include "TrackerParameter.h"

namespace boost {
	void throw_exception(std::exception const & e) {};
}

namespace sampleTracker {
	TrackerParameter::TrackerParameter(QObject *parent) :
		IModel(parent)
	{
		m_Threshold = 12345;

		_lowH = 0;
		_highH = 255;
		_lowS = 0;
		_highS = 255;
		_lowV = 0;
		_highV = 255;
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
}