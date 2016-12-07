#include "TextureObjectView.h"
#include "Model/BioTracker3TextureObject.h"


TextureObjectView::TextureObjectView(QObject *parent, IController *controller, IModel *model) :
    IViewGraphicsPixmapItem(parent, controller, model)
{
    setZValue(-1);
}

void TextureObjectView::getNotified()
{
    BioTracker3TextureObject *texture = dynamic_cast<BioTracker3TextureObject *>(getModel());
    QPixmap pma;
    pma.convertFromImage(texture->get());
    setPixmap(pma);

    update();
}

void TextureObjectView::connectModelView()
{
    QObject::connect(getModel(), &IModel::notifyView, this, &TextureObjectView::getNotified);

    getNotified();
}
