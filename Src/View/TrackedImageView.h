#ifndef TRACKEDIMAGEVIEW_H
#define TRACKEDIMAGEVIEW_H

#include "Interfaces/IView/IViewOpenGLWidget.h"

#include "QPainter"
#include "Interfaces/IView/IViewWidget.h"

class TrackedImageView : public IViewOpenGLWidget
{
public:
    TrackedImageView(QWidget* parent, IController* controller, IModel* model);
    ~TrackedImageView();

    // QOpenGLWidget interface
protected:
    void paintGL() override;

    // IViewOpenGLWidget interface
public Q_SLOTS:
    void getNotified() override;
};

#endif // TRACKEDIMAGEVIEW_H
