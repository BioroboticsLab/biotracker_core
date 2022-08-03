/****************************************************************************
 **
 ** This file is part of the BioTracker Framework
 ** by Andreas Jörg
 **
 ****************************************************************************/

#ifndef CONTROLLERTEXTUREOBJECT_H
#define CONTROLLERTEXTUREOBJECT_H

#include "IControllerCfg.h"
#include "opencv2/core/core.hpp"
#include "QString"
#include "Model/TextureObject.h"

#include <QAbstractListModel>
#include "QStringList"
#include "QStringListModel"
#include "QPointer"


/**
 * This is the Controller class of the software component TextureObject. This
 * component is responsible for rendering cv::Mats on a disply canvas. Other
 * components can trigger the SLOT receiveCvMat(std::shard_ptr<cv::Mat> mat,
 * QString name) in order to render their images. Usually this would be the
 * component MediaPlayer which will send the original cv::Mat from an
 * ImageStream to controller class ControllerTextureObject. An other component
 * is the BioTracker Plugin. If a cv::Mat is manipulated by a Tracking
 * Algorithm the Plugin is able to send that cv::Mat to this component. The
 * Parameter name will be listed in the combobox widget on the MainWindow
 * widget. The ControllerTextureObject class controlls the of the
 * TrextureObject Component.
 */
class ControllerTextureObject : public IControllerCfg
{
    Q_OBJECT
public:
    ControllerTextureObject(
        QObject*              parent  = 0,
        IBioTrackerContext*   context = 0,
        ENUMS::CONTROLLERTYPE ctr     = ENUMS::CONTROLLERTYPE::NO_CTR);

    /**
     * If the user changes the combobox widget for the rendered cv::Mat in the
     * MainWindow this methode will change the corresponding IModel class
     * TextureObject.
     */
    void changeTextureModel(QString name);

    // IController interface
public:
    void connectControllerToController() override;

    QAbstractListModel* textureNamesModel();
    bool hasTexture(QString name);

public Q_SLOTS:
    void setTextureNames(QVector<QString> names);
    void updateTexture(QString name, cv::Mat img);
    void updateTextures(QMap<QString, cv::Mat> textures);

protected:
    void createModel() override;
    void createView() override;
    void connectModelToController() override;

private:
    void createNewTextureObjectModel(QString name);
    void changeTextureView(IModel* model);

private:
    QMap<QString, QPointer<TextureObject>> m_TextureObjects;

    QString m_DefaultTextureName = "Original";

    QPointer<QStringListModel> m_TextureViewNamesModel;
};

#endif // CONTROLLERTEXTUREOBJECT_H
