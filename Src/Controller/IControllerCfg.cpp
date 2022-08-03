#include "IControllerCfg.h"

IControllerCfg::IControllerCfg(QObject*              parent,
                               IBioTrackerContext*   context,
                               ENUMS::CONTROLLERTYPE ctr)
: IController(parent, context, ctr)
{
}

void IControllerCfg::setConfig(Config* cfg)
{
    _cfg = cfg;
}

Config* IControllerCfg::getConfig()
{
    return _cfg;
}