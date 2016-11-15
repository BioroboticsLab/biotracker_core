#ifndef CONTROLLERTEXTUREOBJECT_H
#define CONTROLLERTEXTUREOBJECT_H

#include "Interfaces/icontroller.h"
#include "opencv2/core/core.hpp"
#include "QString"
#include "Model/BioTracker3TextureObject.h"

#include "QStringList"
#include "QStringListModel"


class ControllerTextureObject : public IController
{
    Q_OBJECT
public:
    ControllerTextureObject(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);

    void changeTextureModel(QString name);

    // IController interface
public:
    void connectController() override;

public Q_SLOTS:
    void receiveCvMat(cv::Mat mat, QString name);

Q_SIGNALS:
    void emitNameOfTextureObject(QString name);

protected:
    void createModel() override;
    void createView() override;
    void connectModelController() override;

private:
    void checkIfTextureModelExists(QString name);
    void createNewTextureObjectModel(QString name);
    void changeTextureView(IModel *model);

private:
    QMap<QString, BioTracker3TextureObject *> m_TextureObjects;

    QString m_DefaultTextureName = "No View";

    QStringList m_TextureViewNames;
    QStringListModel *m_TextureViewNamesModel;

};

#endif // CONTROLLERTEXTUREOBJECT_H
