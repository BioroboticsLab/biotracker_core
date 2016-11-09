#include "ControllerTextureObject.h"
#include "../biotracker_gui/biotracker/View/BioTracker3VideoView.h"
#include "../biotracker_gui/biotracker/View/BioTracker3MainWindow.h"
#include "Model/BioTracker3TextureObject.h"
#include "Controller/ControllerPlayer.h"
#include "Model/BioTracker3Player.h"

ControllerTextureObject::ControllerTextureObject(QObject *parent, IBioTrackerContext *context) :
    IController(parent, context)
{

}

void ControllerTextureObject::connectViewToMainWindow(IController *controller)
{
    IView *view = controller->getView();
    static_cast<BioTracker3MainWindow *>(view)->addVideoView(m_View);
}

void ControllerTextureObject::connectToOtherController(IController *controller)
{
    ControllerPlayer *ctr = dynamic_cast<ControllerPlayer *>(controller);
    BioTracker3Player *player = dynamic_cast<BioTracker3Player *>(ctr->getModel());

    QObject::connect(player, &BioTracker3Player::emitCurrentFrame, this, &ControllerTextureObject::receiveCvMat);

}

void ControllerTextureObject::receiveCvMat(cv::Mat mat)
{
    dynamic_cast<BioTracker3TextureObject *>(m_Model)->set(mat);
}

void ControllerTextureObject::createModel()
{
    m_Model = new BioTracker3TextureObject(this);
}

void ControllerTextureObject::createView()
{
    m_View = new BioTracker3VideoView(0, this, m_Model);
}

void ControllerTextureObject::connectModelController()
{

}

void ControllerTextureObject::connectModelView()
{

}
