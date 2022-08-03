#include "ControllerTextureObject.h"
#include "View/GLVideoView.h"
#include "View/MainWindow.h"
#include "Controller/ControllerGraphicScene.h"
#include "Controller/ControllerPlayer.h"
#include "Model/MediaPlayerStateMachine/MediaPlayerStateMachine.h"
#include "View/TextureObjectView.h"

#include <QDebug>

ControllerTextureObject::ControllerTextureObject(QObject*              parent,
                                                 IBioTrackerContext*   context,
                                                 ENUMS::CONTROLLERTYPE ctr)
: IControllerCfg(parent, context, ctr)
{
    m_TextureViewNamesModel = new QStringListModel();
    createNewTextureObjectModel("Original");
}

void ControllerTextureObject::changeTextureModel(QString name)
{
    if (!hasTexture(name)) {
        qCritical().noquote()
            << QString("Invalid texture name '%1'").arg(name);
        return;
    }

    m_Model = m_TextureObjects.value(name);

    changeTextureView(m_Model);
}

void ControllerTextureObject::connectControllerToController()
{
    IController* ctr = m_BioTrackerContext->requestController(
        ENUMS::CONTROLLERTYPE::PLAYER);
    QPointer<ControllerPlayer> ctrPlayer = qobject_cast<ControllerPlayer*>(
        ctr);

    QPointer<VideoControllWidget> videoView =
        dynamic_cast<VideoControllWidget*>(ctrPlayer->getView());
    videoView->setVideoViewComboboxModel(m_TextureViewNamesModel);

    IController* ctrG = m_BioTrackerContext->requestController(
        ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);
    QPointer<ControllerGraphicScene> ctrGraphics =
        qobject_cast<ControllerGraphicScene*>(ctrG);
    QGraphicsPixmapItem* item = dynamic_cast<QGraphicsPixmapItem*>(m_View);

    ctrGraphics->addTextureObject(item);
}

void ControllerTextureObject::setTextureNames(QVector<QString> names)
{
    changeTextureModel("Original");

    for (auto name : m_TextureViewNamesModel->stringList()) {
        if (name != "Original") {
            m_TextureObjects.remove(name);
        }
    }

    m_TextureViewNamesModel->setStringList({"Original"});

    for (auto name : names) {
        createNewTextureObjectModel(name);
    }
}

void ControllerTextureObject::updateTexture(QString name, cv::Mat img)
{
    if (name.isEmpty() || !hasTexture(name)) {
        qCritical().noquote()
            << QString("Invalid texture name '%1'").arg(name);
        return;
    }

    if (!hasTexture(name)) {
        createNewTextureObjectModel(name);
    }

    m_TextureObjects.value(name)->set(img);
}

void ControllerTextureObject::updateTextures(QMap<QString, cv::Mat> textures)
{
    for (const auto name : textures.keys()) {
        updateTexture(name, textures[name]);
    }
}

void ControllerTextureObject::createModel()
{
    createNewTextureObjectModel(m_DefaultTextureName);

    m_Model = m_TextureObjects.value(m_DefaultTextureName);
}

void ControllerTextureObject::createView()
{
    m_View = new TextureObjectView(this, this, m_Model);
}

void ControllerTextureObject::connectModelToController()
{
}

bool ControllerTextureObject::hasTexture(QString name)
{
    if (name.isEmpty()) {
        return false;
    }

    for (int i = 0; i < m_TextureViewNamesModel->rowCount(); ++i) {
        if (m_TextureViewNamesModel->data(m_TextureViewNamesModel->index(i))
                .toString() == name) {
            return true;
        }
    }

    return false;
}

void ControllerTextureObject::createNewTextureObjectModel(QString name)
{
    m_TextureObjects.insert(name, new TextureObject(this, name));

    if (m_TextureViewNamesModel->insertRow(
            m_TextureViewNamesModel->rowCount())) {
        auto index = m_TextureViewNamesModel->index(
            m_TextureViewNamesModel->rowCount() - 1);
        m_TextureViewNamesModel->setData(index, name);
    } else {
        qFatal("Failed to add new texture model");
    }
}

void ControllerTextureObject::changeTextureView(IModel* model)
{
    m_View->setNewModel(model);
}

QAbstractListModel* ControllerTextureObject::textureNamesModel()
{
    return m_TextureViewNamesModel;
}
