#include "TrackerParameter.h"
#include "util/singleton.h"

TrackerParameter::TrackerParameter(QObject *parent) :
    IModel(parent)
{
	_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CONFIGPARAM::CONFIG_INI_FILE);
	//_settings = new BioTracker::Core::Settings(CONFIGPARAM::CONFIG_INI_FILE);

	m_BinarizationThreshold = _settings->getValueOrDefault(TRACKERPARAM::THRESHOLD_BINARIZING, 40);
	m_SizeErode = _settings->getValueOrDefault(TRACKERPARAM::SIZE_ERODE, 8);
	m_SizeDilate = _settings->getValueOrDefault(TRACKERPARAM::SIZE_DILATE, 8);
	m_MinBlobSize = _settings->getValueOrDefault(TRACKERPARAM::MIN_BLOB_SIZE, 40);
	m_MaxBlobSize = _settings->getValueOrDefault(TRACKERPARAM::MAX_BLOB_SIZE, 999999);

	m_mog2History = _settings->getValueOrDefault(TRACKERPARAM::BG_MOG2_HISTORY, 200);
	m_mog2VarThresh = _settings->getValueOrDefault(TRACKERPARAM::BG_MOG2_VAR_THRESHOLD, 64);
	m_mog2BackgroundRatio = _settings->getValueOrDefault(TRACKERPARAM::BG_MOG2_BACKGROUND_RATIO, 0.05);

	m_noFish = _settings->getValueOrDefault(FISHTANKPARAM::FISHTANK_FISH_AMOUNT, 2);
	m_doNetwork = _settings->getValueOrDefault(FISHTANKPARAM::FISHTANK_ENABLE_NETWORKING, false);
	m_networkPort = _settings->getValueOrDefault(FISHTANKPARAM::FISHTANK_NETWORKING_PORT, 54444);


	m_areaWidth = _settings->getValueOrDefault<int>(FISHTANKPARAM::FISHTANK_AREA_WIDTH, 80);
	m_areaHeight = _settings->getValueOrDefault<int>(FISHTANKPARAM::FISHTANK_AREA_HEIGHT, 80);

	m_Threshold = 12345;

	m_doBackground = true;
	m_sendImage = 0; //Send no image
	m_resetBackground = false;

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
	_settings->setParam(TRACKERPARAM::THRESHOLD_BINARIZING, x);
	Q_EMIT notifyView();
}

int TrackerParameter::getBinarizationThreshold()
{
	return m_BinarizationThreshold;
}