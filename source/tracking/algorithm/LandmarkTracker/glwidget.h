#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>

class ToolWindow;

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void drawingAxes();
    void drawCube (float red, float green, float blue);
	void drawingCubes();


signals:

public slots:

    //mouse click and move events
    void mouseMoveEvent		(QMouseEvent * e);
    void mousePressEvent	(QMouseEvent * e);
    void mouseReleaseEvent  (QMouseEvent * e);
    void wheelEvent(QWheelEvent *e);

private:

    GLfloat rotX;
    GLfloat rotY;
    GLfloat rotZ;
    QPoint lastPos;
    GLfloat zoomFactor;

	const ToolWindow * const parent_tw;

};

#endif // GLWIDGET_H
