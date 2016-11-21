#ifndef BIOTRACKER3VIDEOVIEW_H
#define BIOTRACKER3VIDEOVIEW_H

#include "Interfaces/IView/IViewOpenGLWidget.h"

#include "QPainter"
#include "Interfaces/IView/IViewWidget.h"
#include "Model/BioTracker3TextureObject.h"

class BioTracker3VideoView: public IViewOpenGLWidget
{
public:
    BioTracker3VideoView(QWidget *parent, IController *controller = 0, IModel *model = 0);
    ~BioTracker3VideoView();

private:
    void paintGL() override;


public Q_SLOTS:
    void getNotified();


};

#endif // BIOTRACKER3VIDEOVIEW_H
