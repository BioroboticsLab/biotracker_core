#include "TextureObjectView.h"
#include "Model/TextureObject.h"


TextureObjectView::TextureObjectView(QObject *parent, IController *controller, IModel *model) :
    IViewGraphicsPixmapItem(parent, controller, model)
{
    setZValue(-1);
}

void TextureObjectView::getNotified()
{
    TextureObject *texture = dynamic_cast<TextureObject *>(getModel());
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
