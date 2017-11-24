#include "TrackerParameter.h"
#include "util/singleton.h"
#include "Model/TrackingAlgorithm/property/ParamNames.h"

TrackerParameter::TrackerParameter(QObject *parent) :
    IModel(parent)
{
	_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(LUKASKANADE::CONFIGPARAM::CONFIG_INI_FILE);
	//_settings = new BioTracker::Core::Settings(CONFIGPARAM::CONFIG_INI_FILE);

	m_wndSize = _settings->getValueOrDefault(LUKASKANADE::TRACKERPARAM::WNDSIZE, 31);


    Q_EMIT notifyView();
}
