#include "icontroller.h"
#include "IBioTrackerContext.h"


IController::IController(QObject *parent, IBioTrackerContext *context) : QObject(parent),
    m_BioTrackerContext(context)
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
}

void IController::connectViewToMainWindow(IController *controller)
{

}

void IController::connectToOtherController(IController *controller)
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
