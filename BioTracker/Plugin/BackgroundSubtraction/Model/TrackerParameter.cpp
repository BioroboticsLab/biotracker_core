#include "TrackerParameter.h"
#include "util/singleton.h"

TrackerParameter::TrackerParameter(QObject *parent) :
    IModel(parent)
{
	_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CONFIGPARAM::CONFIG_INI_FILE);
	//_settings = new BioTracker::Core::Settings(CONFIGPARAM::CONFIG_INI_FILE);

	_BinarizationThreshold = _settings->getValueOrDefault(TRACKERPARAM::THRESHOLD_BINARIZING, 40);
	_SizeErode = _settings->getValueOrDefault(TRACKERPARAM::SIZE_ERODE, 8);
	_SizeDilate = _settings->getValueOrDefault(TRACKERPARAM::SIZE_DILATE, 8);
	_MinBlobSize = _settings->getValueOrDefault(TRACKERPARAM::MIN_BLOB_SIZE, 40);
	_MaxBlobSize = _settings->getValueOrDefault(TRACKERPARAM::MAX_BLOB_SIZE, 999999);

	_mog2History = _settings->getValueOrDefault(TRACKERPARAM::BG_MOG2_HISTORY, 200);
	_mog2VarThresh = _settings->getValueOrDefault(TRACKERPARAM::BG_MOG2_VAR_THRESHOLD, 64);
	_mog2BackgroundRatio = _settings->getValueOrDefault(TRACKERPARAM::BG_MOG2_BACKGROUND_RATIO, 0.05);

	_doNetwork = _settings->getValueOrDefault(FISHTANKPARAM::FISHTANK_ENABLE_NETWORKING, false);
	_networkPort = _settings->getValueOrDefault(FISHTANKPARAM::FISHTANK_NETWORKING_PORT, 54444);

	_Threshold = 12345;

	_doBackground = true;
	_sendImage = 0; //Send no image
	_resetBackground = false;

    Q_EMIT notifyView();
}

void TrackerParameter::setThreshold(int x)
{
    _Threshold = x;

    Q_EMIT notifyView();
}

int TrackerParameter::getThreshold()
{
    return _Threshold;
}

void TrackerParameter::setBinarizationThreshold(int x)
{
	_BinarizationThreshold = x;
	_settings->setParam(TRACKERPARAM::THRESHOLD_BINARIZING, x);
	Q_EMIT notifyView();
}

int TrackerParameter::getBinarizationThreshold()
{
	return _BinarizationThreshold;
}