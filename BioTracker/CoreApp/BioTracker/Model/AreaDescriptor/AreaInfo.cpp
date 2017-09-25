#include "AreaInfo.h"

#include "Rectification.h"

#include "settings/Settings.h"
#include "util/misc.h"
#include "util/types.h"

AreaInfo::AreaInfo(QObject *parent) :
	IModelAreaDescriptor(parent)
{

	_rect = std::make_shared<AreaInfoElement>();
	_apperture = std::make_shared<AreaInfoElement>();

	std::vector<cv::Point> p = Rectification::instance().getDefaultArena();
	_rect->setVertices(p);


	BioTracker::Core::Settings *_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
	std::string arena = _settings->getValueOrDefault<std::string>(AREADESCRIPTOR::CN_APPERTURE, "15,15;15,105;105,105;105,15");
	p = stringToCVPointVec(arena);
	_apperture->setVertices(p);
}

void AreaInfo::updateRectification() {

	Rectification::instance().setArea(_rect->getQVertices());
	Rectification::instance().setupRecitification(100, 100, 2040, 2040);
}