#include "GLVideoView.h"

#include <util/ScreenHelper.h>
#include "Interfaces/IController/IController.h"

#include "Model/TextureObject.h"

GLVideoView::GLVideoView(QWidget*     parent,
                         IController* controller,
                         IModel*      model)
: IViewOpenGLWidget(parent, controller, model)
{
}

GLVideoView::~GLVideoView()
{
}

void GLVideoView::paintGL()
{
    TextureObject* textureObject = dynamic_cast<TextureObject*>(getModel());
    QImage         img           = textureObject->get();

    QPainter painter(this);

    painter.setBrush(QColor(0, 0, 0));
    painter.drawRect(QRect(0, 0, this->width(), this->height()));
    painter.setBrush(QColor(0, 0, 0, 0));

    QRect       window;
    QRect       viewport;
    const float viewport_skew =
        BioTracker::Core::ScreenHelper::calculate_viewport(
            textureObject->width(),
            textureObject->height(),
            this->width(),
            this->height(),
            window,
            viewport);

    painter.setWindow(window);
    painter.setViewport(viewport);

    QRectF target(0, 0, textureObject->width(), textureObject->height());
    QRectF source(0, 0, textureObject->width(), textureObject->height());

    painter.drawImage(target, img, source);
    QPainter p(this);
}

void GLVideoView::getNotified()
{
    this->update();
}
