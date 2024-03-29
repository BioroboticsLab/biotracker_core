#include "ControllerTrackedComponentCore.h"
#include "ControllerMainWindow.h"
#include "ControllerCoreParameter.h"
#include "ControllerPlayer.h"
#include "Model/null_Model.h"
#include "Model/CoreParameter.h"
#include "View/TrackedComponentView.h"
#include "View/CoreParameterView.h"
#include "util/types.h"
#include "qdebug.h"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"
#include "Interfaces/IView/IViewTrackedComponent.h"
#include "Controller/ControllerCommands.h"

ControllerTrackedComponentCore::ControllerTrackedComponentCore(
    QObject*              parent,
    IBioTrackerContext*   context,
    ENUMS::CONTROLLERTYPE ctr)
: IControllerCfg(parent, context, ctr)
{
    m_View  = nullptr;
    m_Model = nullptr;
}

void ControllerTrackedComponentCore::createView()
{
    // This occurs when there has been a previous plugin to visualize
    if (m_View != nullptr)
        delete m_View;

    m_View = new TrackedComponentView(0, this, m_Model);
}

void ControllerTrackedComponentCore::connectModelToController()
{
}

void ControllerTrackedComponentCore::connectControllerToController()
{
    IController* ctr = m_BioTrackerContext->requestController(
        ENUMS::CONTROLLERTYPE::MAINWINDOW);
    QPointer<ControllerMainWindow> ctrMainWindow =
        qobject_cast<ControllerMainWindow*>(ctr);

    // connect to view
    TrackedComponentView* view = static_cast<TrackedComponentView*>(m_View);
    QObject::connect(view,
                     SIGNAL(emitAddTrajectory(QPoint, int)),
                     this,
                     SIGNAL(emitAddTrajectory(QPoint, int)));
    QObject::connect(view,
                     SIGNAL(emitSwapIds(IModelTrackedTrajectory*,
                                        IModelTrackedTrajectory*)),
                     this,
                     SIGNAL(emitSwapIds(IModelTrackedTrajectory*,
                                        IModelTrackedTrajectory*)));
    QObject::connect(this,
                     SIGNAL(emitDimensionUpdate(int, int)),
                     view,
                     SLOT(rcvDimensionUpdate(int, int)));

    QObject::connect(this,
                     SIGNAL(emitAddTrack()),
                     view,
                     SLOT(addTrajectory()));
    QObject::connect(this,
                     SIGNAL(emitDeleteSelectedTracks()),
                     view,
                     SLOT(removeTrajectories()));
    QObject::connect(this,
                     SIGNAL(emitSelectAll()),
                     view,
                     SLOT(receiveSelectAll()));
    QObject::connect(this, SIGNAL(emitSwapIdsToView()), view, SLOT(swapIds()));
    QObject::connect(this,
                     SIGNAL(emitChangeColorBorder()),
                     view,
                     SLOT(receiveColorChangeBorderSelected()));
    QObject::connect(this,
                     SIGNAL(emitChangeColorFill()),
                     view,
                     SLOT(receiveColorChangeBrushSelected()));

    // connect to update track number in core params
    IController* ctrICP = m_BioTrackerContext->requestController(
        ENUMS::CONTROLLERTYPE::COREPARAMETER);
    QPointer<ControllerCoreParameter> ctrCP =
        qobject_cast<ControllerCoreParameter*>(ctrICP);
    QObject::connect(this,
                     SIGNAL(emitTrackNumber(int)),
                     ctrCP,
                     SLOT(receiveTrackNumber(int)));

    // connect to commandcontroller
    IController* ctrICC = m_BioTrackerContext->requestController(
        ENUMS::CONTROLLERTYPE::COMMANDS);
    QPointer<ControllerCommands> ctrCC = qobject_cast<ControllerCommands*>(
        ctrICC);
    QObject::connect(this,
                     SIGNAL(emitAddTrajectory(QPoint, int)),
                     ctrCC,
                     SLOT(receiveAddTrackCommand(QPoint, int)));
    QObject::connect(
        this,
        SIGNAL(emitRemoveTrajectory(IModelTrackedTrajectory*)),
        ctrCC,
        SLOT(receiveRemoveTrackCommand(IModelTrackedTrajectory*)));
    QObject::connect(
        this,
        SIGNAL(emitRemoveTrackEntity(IModelTrackedTrajectory*, uint)),
        ctrCC,
        SLOT(receiveRemoveTrackEntityCommand(IModelTrackedTrajectory*, uint)));
    QObject::connect(this,
                     SIGNAL(emitMoveElement(IModelTrackedTrajectory*,
                                            QPoint,
                                            QPoint,
                                            uint,
                                            int)),
                     ctrCC,
                     SLOT(receiveMoveElementCommand(IModelTrackedTrajectory*,
                                                    QPoint,
                                                    QPoint,
                                                    uint,
                                                    int)));
    QObject::connect(this,
                     SIGNAL(emitSwapIds(IModelTrackedTrajectory*,
                                        IModelTrackedTrajectory*)),
                     ctrCC,
                     SLOT(receiveSwapIdCommand(IModelTrackedTrajectory*,
                                               IModelTrackedTrajectory*)));
    QObject::connect(
        this,
        SIGNAL(emitToggleFixTrack(IModelTrackedTrajectory*, bool)),
        ctrCC,
        SLOT(receiveFixTrackCommand(IModelTrackedTrajectory*, bool)));
    QObject::connect(this,
                     SIGNAL(emitEntityRotation(IModelTrackedTrajectory*,
                                               double,
                                               double,
                                               uint)),
                     ctrCC,
                     SLOT(receiveEntityRotation(IModelTrackedTrajectory*,
                                                double,
                                                double,
                                                uint)));

    // connect to controllerPlayer
    IController* ctrIP = m_BioTrackerContext->requestController(
        ENUMS::CONTROLLERTYPE::PLAYER);
    QPointer<ControllerPlayer> ctrP = qobject_cast<ControllerPlayer*>(ctrIP);
    QObject::connect(this,
                     &ControllerTrackedComponentCore::emitGoToFrame,
                     ctrP,
                     &ControllerPlayer::setGoToFrame);

    // Tell the Visualization to reset upon loading a new plugin
    QObject::connect(ctrMainWindow,
                     &ControllerMainWindow::emitTrackLoaded,
                     this,
                     &ControllerTrackedComponentCore::receiveOnPluginLoaded);
}

void ControllerTrackedComponentCore::receiveOnPluginLoaded()
{
    receiveUpdateView();
}

void ControllerTrackedComponentCore::setCorePermission(
    std::pair<ENUMS::COREPERMISSIONS, bool> permission)
{
    if (dynamic_cast<TrackedComponentView*>(m_View)) {
        dynamic_cast<TrackedComponentView*>(m_View)->setPermission(permission);
    } else {
        // qDebug() << "no view yet";
        // This should never happen, actually
        assert(false);
    }
}

void ControllerTrackedComponentCore::receiveUpdateView()
{
    TrackedComponentView* compView = dynamic_cast<TrackedComponentView*>(
        m_View);
    compView->getNotified();
    // signal the core parameter controller to update the track number
    IModelTrackedTrajectory* model = dynamic_cast<IModelTrackedTrajectory*>(
        getModel());
    int trackNumber = model->validCount();
    emitTrackNumber(trackNumber);
}

void ControllerTrackedComponentCore::createModel()
{
    // This controller gets his model (via addModel()) from the corresponding
    // tracked-component-controller when a plugin is loaded
    m_Model = new null_Model();
}

IView* ControllerTrackedComponentCore::getTrackingElementsWidgetCore()
{
    return m_View;
}

IModel* ControllerTrackedComponentCore::getCoreParameter()
{
    IController* ctr = m_BioTrackerContext->requestController(
        ENUMS::CONTROLLERTYPE::COREPARAMETER);
    QPointer<ControllerCoreParameter> ctrCP =
        qobject_cast<ControllerCoreParameter*>(ctr);
    return ctrCP->getModel();
}

void ControllerTrackedComponentCore::addModel(IModel* model)
{
    m_Model = model;
    m_View->setNewModel(m_Model);

    TrackedComponentView* view = dynamic_cast<TrackedComponentView*>(m_View);

    // signal initial track number to core params
    IModelTrackedTrajectory* iModel = dynamic_cast<IModelTrackedTrajectory*>(
        getModel());
    if (iModel) {
        int trackNumber = iModel->validCount();
        emitTrackNumber(trackNumber);
    }
}

void ControllerTrackedComponentCore::receiveVisualizeTrackingModel(
    uint framenumber)
{
    // signal the view to update track entities
    TrackedComponentView* compView = dynamic_cast<TrackedComponentView*>(
        m_View);
    compView->updateShapes(framenumber);
    // signal the core parameter controller to update the track number
    IModelTrackedTrajectory* model = dynamic_cast<IModelTrackedTrajectory*>(
        getModel());
    if (model) {
        int trackNumber = model->validCount();
        emitTrackNumber(trackNumber);
    }
}
