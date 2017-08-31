#include "AreaInfo.h"

#include "Model/TrackingAlgorithm/property/Rectification.h"

#include "settings/Settings.h"
#include "Model/TrackingAlgorithm/property/ParamNames.h"
#include "util/misc.h"

AreaInfo::AreaInfo(QObject *parent) :
	IModel(parent)
{

	_rect = std::make_shared<AreaInfoElement>();
	_apperture = std::make_shared<AreaInfoElement>();

	std::vector<cv::Point> p = Rectification::instance().getDefaultArena();
	_rect->setVertices(p);


	BioTracker::Core::Settings *_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CONFIGPARAM::CONFIG_INI_FILE);
	std::string arena = _settings->getValueOrDefault<std::string>(TRACKERPARAM::CN_APPERTURE, "15,15;15,105;105,105;105,15");
	p = stringToCVPointVec(arena);
	_apperture->setVertices(p);
}