#ifndef CONTROLLERTEXTUREOBJECT_H
#define CONTROLLERTEXTUREOBJECT_H

#include "Interfaces/IController/IController.h"
#include "opencv2/core/core.hpp"
#include "QString"
#include "Model/TextureObject.h"

#include "QStringList"
#include "QStringListModel"
#include "QPointer"


class ControllerTextureObject : public IController
{
    Q_OBJECT
public:
    ControllerTextureObject(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);

    void changeTextureModel(QString name);

    // IController interface
public:
    void connectControllerToController() override;

public Q_SLOTS:
    void receiveCvMat(std::shared_ptr<cv::Mat> mat, QString name);

Q_SIGNALS:
    void emitNameOfTextureObject(QString name);

protected:
    void createModel() override;
    void createView() override;
    void connectModelToController() override;

private:
    void checkIfTextureModelExists(QString name);
    void createNewTextureObjectModel(QString name);
    void changeTextureView(IModel *model);

private:
    QMap<QString, QPointer< TextureObject > > m_TextureObjects;

    QString m_DefaultTextureName = "No View";

    QStringList m_TextureViewNames;
    QPointer< QStringListModel > m_TextureViewNamesModel;

};

#endif // CONTROLLERTEXTUREOBJECT_H
