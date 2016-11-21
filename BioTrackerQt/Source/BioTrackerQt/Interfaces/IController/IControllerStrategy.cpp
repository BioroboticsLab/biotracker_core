#include "IControllerStrategy.h"
#include "Interfaces/IController/icontroller.h"


IControllerStrategy::IControllerStrategy(QObject *parent, IController *controller) : QObject(parent)
{
    m_Controller = controller;
}

void IControllerStrategy::execute()
{

}
