#include "glwidget.h"
#include <iostream>
#include "toolwindow.h"

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>	
#endif

GLWidget::GLWidget(QWidget *parent) :
	QGLWidget(parent),parent_tw(static_cast<ToolWindow*>(parent)),zoomFactor(-90),rotX(0.5),rotY(50),rotZ(0)
{}

void GLWidget::initializeGL()
{
    glClearColor(0.2, 0.2, 0.2, 1);

    glShadeModel(GL_SMOOTH);
    glClearDepth(10.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glTranslated(-15,-10, zoomFactor);
//    glRotatef(0, 1.0, 1.0, 0.0);
//    glRotatef(15, 0.0, 1.0, 0.0);
    glRotatef(rotX, 1.0, 0.0, 0.0);
    glRotatef(rotY, 0.0, 1.0, 0.0);
    glRotatef(rotZ, 0.0, 0.0, 1.0);

    drawingAxes();
	drawingCubes();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.01, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(0,0,5, 0,0,0, 0,1,0);
}

void GLWidget::drawingAxes()
{
    int lines=256;

	//std::cout<<"DRAWING AXES"<<std::endl;
	
    glBegin(GL_LINES);

        /******draw red-axis********/
        glColor3f(1.0f, 0.0f, 0.0f);
            for(int y = 0; y<257; y++){
                if(y%lines==0){
                    for(int x = 0; x<257; x++){
                        if(x%lines==0){
                             glVertex3f(0.0f, float(x)/10.0f, float(y)/10.0f);
                             glVertex3f(25.6f, float(x)/10.0f, float(y)/10.0f);
                        }
                    }
                }
            }

        /******draw green-axis********/
        glColor3f(0.0f, 1.0f, 0.0f);
            for(int w = 0; w<257; w++){
                if(w%lines==0){
                    for(int u = 0; u<257; u++){
                        if(u%lines==0){
                            glVertex3f(float(w)/10.0f, 0.0f, float(u)/10.0f);
                            glVertex3f(float(w)/10.0f, 25.6f, float(u)/10.0f);
                        }
                    }
                }
        }

        /******draw blue-axis********/
        glColor3f(0.0f, 0.0f, 1.0f);
            for(int v = 0; v<257; v++){
                if(v%lines==0){
                    for(int z = 0; z<257; z++){
                        if(z%lines==0){
                            glVertex3f(float(v)/10.0f, float(z)/10.0f, 0.0f);
                            glVertex3f(float(v)/10.0f, float(z)/10.0f, 25.6f);
                        }
                    }
            }
        }
    glEnd();

}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
   GLfloat dx = (GLfloat) (e->x() - lastPos.x()) / width();
   GLfloat dy = (GLfloat) (e->y() - lastPos.y()) / height();

   if (e->buttons() & Qt::LeftButton) {
    rotX += 180 * dy;
    rotY += 180 * dx;
    updateGL();
   }
   else if (e->buttons() & Qt::RightButton) {
    rotX += 180 * dy;
    rotZ += 180 * dx;
    updateGL();
   }

   lastPos = e->pos();
}

void GLWidget::mouseReleaseEvent  (QMouseEvent * e)
{
    std::cout<<"X: " <<rotX<<std::endl;
    std::cout<<"Y: " <<rotY<<std::endl;
    std::cout<<"Z: " <<rotZ<<std::endl;
}

void GLWidget::wheelEvent(QWheelEvent *e)
{
    zoomFactor += 0.1 * e->delta();
    updateGL();
}

void GLWidget::drawingCubes()
{
	for(const auto &p:parent_tw->getRGBMap())
	{
		drawCube(float (p.first.val[2])/10, float (p.first.val[1])/10, float (p.first.val[0])/10);
	}
}

void GLWidget::drawCube (float red, float green, float blue)
{

     glBegin(GL_QUADS);
         glColor3f(red/25.5,green/25.5,blue/25.5);
        //Front Face
           glVertex3f(red, green,  blue+0.1f);
           glVertex3f(red, green+0.1f,  blue+0.1f);
           glVertex3f(red+0.1f, green+0.1f ,  blue+0.1f );
           glVertex3f(red+0.1f, green,  blue+0.1f );
             // Back Face
           glVertex3f(red, green,  blue);
           glVertex3f(red, green+0.1f ,  blue);
           glVertex3f(red+0.1f , green+0.1f ,  blue);
           glVertex3f(red+0.1f , green,  blue);
             // Top Face
           glVertex3f(red, green+0.1f ,  blue);
           glVertex3f(red, green+0.1f ,  blue+0.1f );
           glVertex3f(red+0.1f , green+0.1f ,  blue);
           glVertex3f(red+0.1f , green+0.1f ,  blue+0.1f );
             // Bottom Face
           glVertex3f(red, green,  blue);
           glVertex3f(red, green,  blue+0.1f );
           glVertex3f(red+0.1f , green,  blue);;
           glVertex3f(red+0.1f , green,  blue+0.1f );
             // Right face
           glVertex3f(red+0.1f , green+0.1f ,  blue+0.1f );
           glVertex3f(red+0.1f , green+0.1f ,  blue);
           glVertex3f(red+0.1f , green,  blue+0.1f );
           glVertex3f(red+0.1f , green,  blue);
             // Left Face
           glVertex3f(red, green+0.1f ,  blue+0.1f );
           glVertex3f(red, green+0.1f ,  blue);
           glVertex3f(red, green,  blue+0.1f );
           glVertex3f(red, green,  blue);
    glEnd();
}
