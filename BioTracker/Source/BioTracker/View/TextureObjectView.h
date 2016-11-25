#ifndef TEXTUREOBJECTVIEW_H
#define TEXTUREOBJECTVIEW_H

#include "Interfaces/IView/IGraphicsPixmapItem.h"

class TextureObjectView : public IGraphicsPixmapItem
{
    Q_OBJECT
public:
    TextureObjectView(QObject *parent = 0, IController *controller = 0, IModel *model = 0);

    // IGraphicsPixmapItem interface
public Q_SLOTS:
    void getNotified() override;

    // IView interface
protected:
    void connectModelView() override;
};

#endif // TEXTUREOBJECTVIEW_H
