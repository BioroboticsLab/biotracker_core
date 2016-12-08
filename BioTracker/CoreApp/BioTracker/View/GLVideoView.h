#ifndef BIOTRACKER3VIDEOVIEW_H
#define BIOTRACKER3VIDEOVIEW_H

#include "Interfaces/IView/IViewOpenGLWidget.h"

#include "QPainter"
#include "Interfaces/IView/IViewWidget.h"
#include "Model/TextureObject.h"

class GLVideoView: public IViewOpenGLWidget
{
public:
    GLVideoView(QWidget *parent, IController *controller = 0, IModel *model = 0);
    ~GLVideoView();

private:
    void paintGL() override;


public Q_SLOTS:
    void getNotified();


};

#endif // BIOTRACKER3VIDEOVIEW_H
