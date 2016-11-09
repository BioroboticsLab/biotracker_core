#include "IControllerStrategy.h"
#include "Interfaces/icontroller.h"


IControllerStrategy::IControllerStrategy(QObject *parent, IController *controller) : QObject(parent)
{
    m_Controller = controller;
}

void IControllerStrategy::execute()
{

}
