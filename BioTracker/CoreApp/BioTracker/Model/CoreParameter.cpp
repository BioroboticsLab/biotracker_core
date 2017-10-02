#include "CoreParameter.h"
#include "util/singleton.h"

CoreParameter::CoreParameter(QObject *parent) :
	IModel(parent)
{
	//_settings = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CONFIGPARAM::CONFIG_INI_FILE);
	//_settings = new BioTracker::Core::Settings(CONFIGPARAM::CONFIG_INI_FILE);

	Q_EMIT notifyView();
}
