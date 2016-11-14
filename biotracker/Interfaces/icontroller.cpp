#include "icontroller.h"
#include "IBioTrackerContext.h"


IController::IController(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) : QObject(parent),
    m_BioTrackerContext(context),
    m_ControllerType(ctr)
{

}

IBioTrackerContext *IController::getBioTrackerContext()
{
    return m_BioTrackerContext;
}

void IController::createModel()
{

}

void IController::createView()
{

}

void IController::connectModelController()
{

}

void IController::createComponents()
{
    createModel();
    createView();
    connectModelController();
    connectModelView();
    callAnOtherController();
}

void IController::connectViewToMainWindow(IController *controller)
{

}

void IController::connectToOtherController(IController *controller)
{

}

void IController::callAnOtherController()
{

}

void IController::addView(IView *view) {
    m_View = view;
}

void IController::addModel(IModel *model) {
    m_Model = model;
}

IModel *IController::getModel() {
    return m_Model;
}

IView *IController::getView() {
    return m_View;
}

ENUMS::CONTROLLERTYPE IController::getControllerType()
{
    return m_ControllerType;
}
