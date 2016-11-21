#include "TextureObjectView.h"
#include "Model/BioTracker3TextureObject.h"


TextureObjectView::TextureObjectView(QObject *parent, IController *controller, IModel *model) :
    IGraphicsPixmapItem(parent, controller, model)
{
    setZValue(-1);
}

void TextureObjectView::getNotified()
{
    BioTracker3TextureObject *texture = dynamic_cast<BioTracker3TextureObject *>(getModel());
    QPixmap *pmap = new QPixmap;
    pmap->convertFromImage(texture->get());
    setPixmap(*pmap);

    update();
}

void TextureObjectView::connectModelView()
{
    QObject::connect(getModel(), &IModel::notifyView, this, &TextureObjectView::getNotified);

    getNotified();
}
