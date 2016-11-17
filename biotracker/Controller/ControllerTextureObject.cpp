#include "ControllerTextureObject.h"
#include "../biotracker_gui/biotracker/View/BioTracker3VideoView.h"
#include "../biotracker_gui/biotracker/View/BioTracker3MainWindow.h"
#include "Controller/ControllerPlayer.h"
#include "Model/BioTracker3Player.h"

ControllerTextureObject::ControllerTextureObject(QObject *parent, IBioTrackerContext *context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr)
{
    m_TextureViewNamesModel= new QStringListModel();
    m_TextureViewNamesModel->setStringList(m_TextureViewNames);
}

void ControllerTextureObject::changeTextureModel(QString name)
{
    if(name == QString("") )
        name = m_DefaultTextureName;

    checkIfTextureModelExists(name);
    m_Model = m_TextureObjects.value(name);

    changeTextureView(m_Model);
}

void ControllerTextureObject::addTextureElementView(IView *view)
{
    //dynamic_cast<BioTracker3VideoView *>(m_View)->addTrackedElementView(view);
}

void ControllerTextureObject::connectController()
{
    IController *ctrM = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::MAINWINDOW);
    BioTracker3MainWindow *mainWin = dynamic_cast<BioTracker3MainWindow *>(ctrM->getView());
    mainWin->addVideoView(m_View);


    IController *ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
    ControllerPlayer *ctrPlayer = dynamic_cast<ControllerPlayer *>(ctr);

    BioTracker3Player *player = dynamic_cast<BioTracker3Player *>(ctrPlayer->getModel());
    QObject::connect(player, &BioTracker3Player::emitCurrentFrame, this, &ControllerTextureObject::receiveCvMat);


    BioTracker3VideoControllWidget *videoView = dynamic_cast<BioTracker3VideoControllWidget *> (ctrPlayer->getView());
    videoView->setVideoViewComboboxModel(m_TextureViewNamesModel);
}

void ControllerTextureObject::receiveCvMat(cv::Mat mat, QString name)
{
    checkIfTextureModelExists(name);

    m_TextureObjects.value(name)->set(mat);

}

void ControllerTextureObject::createModel()
{
    createNewTextureObjectModel(m_DefaultTextureName);

    m_Model = m_TextureObjects.value(m_DefaultTextureName);
}

void ControllerTextureObject::createView()
{
    m_View = new BioTracker3VideoView(0, this, m_Model);
}

void ControllerTextureObject::connectModelController()
{

}

void ControllerTextureObject::checkIfTextureModelExists(QString name)
{
    if(name == QString("") )
        name = m_DefaultTextureName;


    bool itemIsInList = false;
    for (int i = 0 ; i < m_TextureViewNames.size(); ++i) {

        if (m_TextureViewNames.at(i) == name) {
            itemIsInList = true;
        }
    }

    if (!itemIsInList) {
        createNewTextureObjectModel(name);
    }
}

void ControllerTextureObject::createNewTextureObjectModel(QString name)
{    
    BioTracker3TextureObject *newTextureModel = new BioTracker3TextureObject(this, name);
    m_TextureObjects.insert(name, newTextureModel);
    m_TextureViewNames.append(name);
    m_TextureViewNamesModel->setStringList(m_TextureViewNames);

}

void ControllerTextureObject::changeTextureView(IModel *model)
{
    m_View->setNewModel(model);

}
