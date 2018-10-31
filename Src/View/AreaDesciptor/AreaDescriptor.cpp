#include "AreaDescriptor.h"
#include "Controller/IControllerCfg.h"

AreaDescriptor::AreaDescriptor(IController *controller, IModel *model) :
    IView(controller, model) {
    _cfg = static_cast<IControllerCfg*>(controller)->getConfig();
};

AreaDescriptor::~AreaDescriptor()
{
}
