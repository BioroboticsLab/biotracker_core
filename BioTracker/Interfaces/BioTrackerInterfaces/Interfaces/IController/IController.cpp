#include "IController.h"


IController::IController(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) : QObject(parent),
    m_BioTrackerContext(context),
    m_ControllerType(ctr)
{

}

void IController::cleanup() {

}

IBioTrackerContext *IController::getBioTrackerContext()
{
    return m_BioTrackerContext.data();
}

void IController::connectModelToController()
{

}

void IController::createComponents()
{
    createModel();
    createView();
    connectModelToController();
}

void IController::connectComponents()
{
    connectControllerToController();
}

void IController::addView(IView *view) {
    m_View = view;
}

void IController::addModel(IModel *model) {
    m_Model = model;
}

IModel *IController::getModel() {
    return m_Model.data();
}

IView *IController::getView() {
    return m_View;
}

ENUMS::CONTROLLERTYPE IController::getControllerType()
{
    return m_ControllerType;
}
