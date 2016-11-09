#include "ControllerContext.h"

ControllerContext::ControllerContext()
{

}

void ControllerContext::executeBioTarckerController()
{
    m_Controller->createComponents();
}

void ControllerContext::setBioTrackerController(IController *controller)
{
    m_Controller = controller;
}

IController *ControllerContext::getBioTrackerController()
{
    return m_Controller;
}
