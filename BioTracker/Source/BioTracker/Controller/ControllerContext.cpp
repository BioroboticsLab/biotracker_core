#include "ControllerContext.h"
#include "null_Controller.h"

ControllerContext::ControllerContext()
{
    m_Controller = new null_Controller();
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
