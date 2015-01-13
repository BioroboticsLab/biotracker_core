#include "glwidget.h"
#include <iostream>
#include "toolwindow.h"

// OS X puts the headers in a different location in the include path than
// Windows and Linux, so we need to distinguish between OS X and the other
// systems.
#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
    , rotX(0.5)
    , rotY(50)
    , rotZ(0)
    , zoomFactor(-90)
    , parent_tw(static_cast<ToolWindow*>(parent))
{}

void GLWidget::initializeGL()
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);

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
    gluPerspective(45, static_cast<float>(w)/h, 0.01, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(0,0,5, 0,0,0, 0,1,0);
}

void GLWidget::drawingAxes()
{
    int lines=256/2;

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
   GLfloat dx = static_cast<GLfloat>(e->x() - lastPos.x()) / width();
   GLfloat dy = static_cast<GLfloat>(e->y() - lastPos.y()) / height();

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

void GLWidget::mouseReleaseEvent  (QMouseEvent * /* e */)
{
    std::cout<<"X: " <<rotX<<std::endl;
    std::cout<<"Y: " <<rotY<<std::endl;
    std::cout<<"Z: " <<rotZ<<std::endl;
}

void GLWidget::wheelEvent(QWheelEvent *e)
{
    zoomFactor += 0.1f * e->delta();
    updateGL();
}

void GLWidget::drawingCubes()
{
	for(const auto &p:parent_tw->getRGBMap())
	{
		drawCube(float (p.first.val[2])/10, float (p.first.val[1])/10, float (p.first.val[0])/10, 25.5/2);
		std::cout<<"Color printed: "<<(int)p.first.val[2]<<", "<< (int)p.first.val[1]<<", "<<(int)p.first.val[0]<<std::endl;
	}
}

void GLWidget::drawCube (float red, float green, float blue, float f)
{
	
	
     glBegin(GL_QUADS);
         glColor3f(red/25.5f,green/25.5f,blue/25.5f);

			//Front Face
			std::cout<<"FRONT FACE: "<<std::endl;
            std::cout<<red<<", "<< green<<", "<<blue+f<<std::endl;
			std::cout<<red<<", "<< green+f<<", "<<blue+f<<std::endl;
		    std::cout<<red+f<<", "<< green<<", "<<blue+f<<std::endl;
            std::cout<<red+f<<", "<< green+f<<", "<<blue<<std::endl;
			
			glVertex3f(red, green,  blue+f);
			glVertex3f(red, green+f,  blue+f);
			glVertex3f(red+f, green+f ,  blue+f );
			glVertex3f(red+f, green,  blue+f );
			

             // Back Face
           glVertex3f(red, green,  blue);
           glVertex3f(red, green+f ,  blue);
           glVertex3f(red+f , green+f ,  blue);
           glVertex3f(red+f , green,  blue);
		   
			// Top Face
           glVertex3f(red, green+f ,  blue);
           glVertex3f(red, green+f ,  blue+f );
		   glVertex3f(red+f , green+f ,  blue+f );
		   glVertex3f(red+f , green+f ,  blue);
             // Bottom Face
           glVertex3f(red, green,  blue);
           glVertex3f(red, green,  blue+f );
           glVertex3f(red+f , green,  blue+f );
		   glVertex3f(red+f , green,  blue);
		   
             // Right face
           glVertex3f(red+f , green+f ,  blue+f );
           glVertex3f(red+f , green+f ,  blue);
		   glVertex3f(red+f , green,  blue);
           glVertex3f(red+f , green,  blue+f );
           
             // Left Face
           glVertex3f(red, green+f ,  blue+f );
           glVertex3f(red, green+f ,  blue);
           glVertex3f(red, green,  blue);
		   glVertex3f(red, green,  blue+f );
		   
    glEnd();

	 /*glBegin(GL_QUADS);
         glColor3f(red/25.5,green/25.5,blue/25.5);

			//Front Face
			std::cout<<"FRONT FACE: "<<std::endl;
            std::cout<<red<<", "<< green<<", "<<blue+f<<std::endl;
			std::cout<<red<<", "<< green+f<<", "<<blue+f<<std::endl;
		    std::cout<<red+f<<", "<< green<<", "<<blue+f<<std::endl;
            std::cout<<red+f<<", "<< green+f<<", "<<blue<<std::endl;

			glVertex3f(red, green,  blue+f);
			glVertex3f(red, green+f,  blue+f);
			glVertex3f(red+f, green,  blue+f );
			glVertex3f(red+f, green+f ,  blue+f );

             // Back Face
           glVertex3f(red, green,  blue);
           glVertex3f(red, green+f ,  blue);
           glVertex3f(red+f , green+f ,  blue);
           glVertex3f(red+f , green,  blue);
             // Top Face
           glVertex3f(red, green+f ,  blue);
           glVertex3f(red, green+f ,  blue+f );
           glVertex3f(red+f , green+f ,  blue);
           glVertex3f(red+f , green+f ,  blue+f );
             // Bottom Face
           glVertex3f(red, green,  blue);
           glVertex3f(red, green,  blue+f );
           glVertex3f(red+f , green,  blue);;
           glVertex3f(red+f , green,  blue+f );
             // Right face
           glVertex3f(red+f , green+f ,  blue+f );
           glVertex3f(red+f , green+f ,  blue);
           glVertex3f(red+f , green,  blue+f );
           glVertex3f(red+f , green,  blue);
             // Left Face
           glVertex3f(red, green+f ,  blue+f );
           glVertex3f(red, green+f ,  blue);
           glVertex3f(red, green,  blue+f );
           glVertex3f(red, green,  blue);
    glEnd();*/
}
