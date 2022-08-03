#include "CoreParameter.h"

CoreParameter::CoreParameter(QObject* parent)
: IModel(parent)
{
    //_settings =
    // BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CONFIGPARAM::CONFIG_INI_FILE);
    //_settings = new BioTracker::Core::Settings(CONFIGPARAM::CONFIG_INI_FILE);

    Q_EMIT notifyView();
}

void CoreParameter::setTrackNumber(int number)
{
    m_trackNumber = number;
    notifyView();
}
