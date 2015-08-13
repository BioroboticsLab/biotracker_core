#include "glwidget.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include "LandmarkTracker.h"



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


//OpenGL init

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
    , rotX(0.5)
    , rotY(50)
    , rotZ(0)
    , moveUpDown(-13)
    , moveLeftRight(-15)
    , zoomFactor(-60)
    //, parent_tw(static_cast<ToolWindow*>(parent)) //ToolWindow nicht mehr gebraucht
{}

void GLWidget::initializeGL() {
    makeCurrent();

    std::cout << "INIT GL..." << std::endl;

    glClearColor(0.2f, 0.2f, 0.2f, 1.f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glShadeModel(GL_SMOOTH);
    glClearDepth(10.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget::paintGL() {

    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    //glTranslated(-15,-10, zoomFactor);
    glTranslated(moveLeftRight, moveUpDown, zoomFactor);

    glRotatef(rotX, 1.0, 0.0, 0.0);
    glRotatef(rotY, 0.0, 1.0, 0.0);
    glRotatef(rotZ, 0.0, 0.0, 1.0);

    updateRGBCube(); //Drawing outter cube and/or cubes
}

void GLWidget::resizeGL(int w, int h) {
    makeCurrent();

    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, static_cast<float>(w)/h, 0.01, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(0,0,5, 0,0,0, 0,1,0);

    viewport_size = this -> size();
}


//Mouse & keyboard events

void GLWidget::mousePressEvent(QMouseEvent *e) {
    lastPos = e->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e) {
    GLfloat dx = static_cast<GLfloat>(e->x() - lastPos.x()) / viewport_size.width();
    GLfloat dy = static_cast<GLfloat>(e->y() - lastPos.y()) /
                 viewport_size.height();

    if (e->buttons() & Qt::LeftButton) {
        rotX += 180 * dy;
        rotY += 180 * dx;
        updateGL();
    } else if (e->buttons() & Qt::RightButton) {
        rotX += 180 * dy;
        rotZ += 180 * dx;
        updateGL();
    }

    lastPos = e->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *) {
    //std::cout << "X: " << rotX << std::endl;
    //std::cout << "Y: " << rotY << std::endl;
    //std::cout << "Z: " << rotZ << std::endl;
}

void GLWidget::wheelEvent(QWheelEvent *e) {
    zoomFactor += 0.05f * e->delta();
    updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_W:
        moveUpDown -= 0.6f;
        break;

    case Qt::Key_S:
        moveUpDown += 0.6f;
        break;

    case Qt::Key_A:
        moveLeftRight += 0.6f;
        break;

    case Qt::Key_D:
        moveLeftRight -= 0.6f;
        break;

    default:
        QGLWidget::keyPressEvent(e);
    }
    updateGL();
}

//Ausgabe für Vector
std::ostream &operator<<(std::ostream &os, const cv::Vec3b &v) {
    return os << "(" << static_cast<unsigned>(v.val[0]) << ", " <<
           static_cast<unsigned>(v.val[1]) << ", " << static_cast<unsigned>
           (v.val[2]) << ")";
}

void GLWidget::clearPixelCubes() {
    rgbMap.clear();
    updateRGBCube();
    updateGL();
}

void GLWidget::computeRgbMap(const cv::Mat &mat) {

    rgbMap.clear();
    rgbValue_max = 0;

    for (int i = 0; i < mat.rows; i++) {
        for (int j = 0; j < mat.cols; j++) {
            rgbMap[mat.at<cv::Vec3b>(i, j)]++;
            rgbValue_max = std::max(rgbValue_max, rgbMap[mat.at<cv::Vec3b>(i, j)]);
        }
    }

    rgbMap_size = rgbMap.size();

    std::cout << "rgbMap Size: " << rgbMap_size << "| Max: " << rgbValue_max <<
              std::endl;


    //Alle Einträge in der rgbMap - RGB Wert und Häufigkeit (sollte bei großen Bilder auskommentiert werden!)

    /*
    for(const auto &v:rgbMap){
    std::cout<<"Vector: "<<v.first<< "| Anzahl:"<<v.second<<std::endl;
    }

    std::cout <<"RGB VALUES COMPUTED!"<<std::endl;
    */

}


// Method to draw RGB-CUBE (Axes)

void GLWidget::drawingAxes() {
    makeCurrent();

    //int lines = 256 / calc_f(rgbMap_size);  // ein Cube - teilen durch x -> mehr cubes
    int lines = 256;

    glShadeModel(GL_SMOOTH);
    glBegin(GL_LINES);

    /******draw red-axis********/
    //glColor3f(1.0f, 0.0f, 0.0f);
    for (int y = 0; y<257; y++) {
        if (y%lines==0) {
            for (int x = 0; x<257; x++) {
                if (x%lines==0) {
                    if (y == 0) {
                        if (x == 0) {
                            colorStart = vec3f { { 0.0f, 0.0f, 0.0f } }; //black
                            colorEnd = vec3f { { 255.0f, 0.0f, 0.0f } }; //red
                        } else {
                            colorStart = vec3f { { 0.0f, 255.0f, 0.0f } }; //green
                            colorEnd = vec3f { { 255.0f, 255.0f, 0.0f } }; //yellow
                        }
                    } else {
                        if (x == 0) {
                            colorStart = vec3f { { 0.0f, 0.0f, 255.0f } }; //blue
                            colorEnd = vec3f { { 255.0f, 0.0f, 255.0f } }; //pink
                        } else {
                            colorStart = vec3f { { 0.0f, 255.0f, 255.0f } }; //blue-green
                            colorEnd = vec3f { { 255.0f, 255.0f, 255.0f } }; //white
                        }
                    }
                    glColor3fv(colorStart.data());
                    glVertex3f(0.0f, float(x) / 10.0f, float(y) / 10.0f);
                    glColor3fv(colorEnd.data());
                    glVertex3f(25.6f, float(x) / 10.0f, float(y) / 10.0f);
                }
            }
        }
    }



    /******draw green-axis********/
    glColor3f(0.0f, 1.0f, 0.0f);
    for (int w = 0; w<257; w++) {
        if (w%lines==0) {
            for (int u = 0; u<257; u++) {
                if (u%lines==0) {
                    if (w == 0) {
                        if (u == 0) {
                            colorStart = vec3f { { 0.0f, 0.0f, 0.0f } }; //black
                            colorEnd = vec3f { { 0.0f, 255.0f, 0.0f } }; //green
                        } else {
                            colorStart = vec3f { { 0.0f, 0.0f, 255.0f } }; //blue
                            colorEnd = vec3f { { 0.0f, 255.0f, 255.0f } }; //blue-green
                        }
                    } else {
                        if (u == 0) {
                            colorStart = vec3f { { 255.0f, 0.0f, 0.0f } }; //red
                            colorEnd = vec3f { { 255.0f, 255.0f, 0.0f } }; //red-green (yellow)
                        } else {
                            colorStart = vec3f { { 255.0f, 0.0f, 255.0f } }; //red-blue
                            colorEnd = vec3f { { 255.0f, 255.0f, 255.0f } }; //white
                        }
                    }
                    glColor3fv(colorStart.data());
                    glVertex3f(float(w)/10.0f, 0.0f, float(u)/10.0f);
                    glColor3fv(colorEnd.data());
                    glVertex3f(float(w)/10.0f, 25.6f, float(u)/10.0f);
                }
            }
        }
    }

    /******draw blue-axis********/
    glColor3f(0.0f, 0.0f, 1.0f);
    for (int v = 0; v<257; v++) {
        if (v%lines==0) {
            for (int z = 0; z<257; z++) {
                if (z%lines==0) {
                    if (v == 0) {
                        if (z == 0) {
                            colorStart = vec3f { { 0.0f, 0.0f, 0.0f } }; //black
                            colorEnd = vec3f { { 0.0f, 0.0f, 255.0f } }; //blue
                        } else {
                            colorStart = vec3f { { 0.0f, 255.0f, 0.0f } }; //green
                            colorEnd = vec3f { { 0.0f, 255.0f, 255.0f } }; //blue-green
                        }
                    } else {
                        if (z == 0) {
                            colorStart = vec3f { { 255.0f, 0.0f, 0.0f } }; //red
                            colorEnd = vec3f { { 255.0f, 0.0f, 255.0f } }; //red-blue
                        } else {
                            colorStart = vec3f { { 255.0f, 255.0f, 0.0f } }; //red-green
                            colorEnd = vec3f { { 255.0f, 255.0f, 255.0f } }; //white
                        }
                    }
                    glColor3fv(colorStart.data());
                    glVertex3f(float(v)/10.0f, float(z)/10.0f, 0.0f);
                    glColor3fv(colorEnd.data());
                    glVertex3f(float(v)/10.0f, float(z)/10.0f, 25.6f);
                }
            }
        }
    }
    glEnd();
}



float GLWidget::calc_f(float pixel_size) {
    return static_cast<float>(ceil(cbrt(pixel_size)));
}

float GLWidget::cube_size(float div_factor, int pixel_count) {
    float cube_size_value = (((pixel_count * 100) / (rgbValue_max))*
                             (25.5f/div_factor)) / 100;

    /*
    std::cout << "DIV FACTOR: " << div_factor << std::endl;
    std::cout << "PIXEL COUNT: " << pixel_count << std::endl;
    std::cout << "CUBE SIZE VALUE: " << cube_size_value << std::endl;
    */

    return cube_size_value;
}

void GLWidget::drawingCubes() {

    for (const auto &p : getRGBMap()) {
        drawCube(float(p.first.val[2]) / 10, float(p.first.val[1]) / 10,
                 float(p.first.val[0]) / 10, cube_size(calc_f(rgbMap_size),
                         static_cast<int>(p.second)));


        //std::cout << "Color printed: " << (int)p.first.val[2] << ", " << (int)p.first.val[1] << ", " << (int)p.first.val[0] << " | " << " rgbMapSize: " << rgbValue_max << std::endl;
        //std::cout << "Teilungsfaktor: " << calc_f(parent_tw->rgbMap_size) << std::endl;
        /*
        std::cout << "DIV FACTOR: " << calc_f(parent_tw->rgbMap_size) << std::endl;
        std::cout << "PIXEL COUNT: " << static_cast<int>(p.second) << std::endl;
        std::cout << "PIXEL VALUE: " << (int)p.first.val[2] << ", " << (int)p.first.val[1] << ", " << (int)p.first.val[0] << std::endl;
        */
    }
}

void GLWidget::drawCube(float red, float green, float blue, float f) {
    makeCurrent();
    //std::cout << "Draw Cube: " << red << ", " << green << ", " << blue << " | " << " f: " << f << std::endl;

    //FILLED CUBE
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_QUADS);
    glColor4f(red/25.5f,green/25.5f,blue/25.5f, 1.0f);

    // Front Face
    glVertex3f(red, green,  blue+f);
    glVertex3f(red, green+f,  blue+f);
    glVertex3f(red+f, green+f ,  blue+f);
    glVertex3f(red+f, green,  blue+f);

    // Back Face
    glVertex3f(red, green,  blue);
    glVertex3f(red, green+f ,  blue);
    glVertex3f(red+f , green+f ,  blue);
    glVertex3f(red+f , green,  blue);

    // Top Face
    glVertex3f(red, green+f ,  blue);
    glVertex3f(red, green+f ,  blue+f);
    glVertex3f(red+f , green+f ,  blue+f);
    glVertex3f(red+f , green+f ,  blue);
    // Bottom Face
    glVertex3f(red, green,  blue);
    glVertex3f(red, green,  blue+f);
    glVertex3f(red+f , green,  blue+f);
    glVertex3f(red+f , green,  blue);

    // Right face
    glVertex3f(red+f , green+f ,  blue+f);
    glVertex3f(red+f , green+f ,  blue);
    glVertex3f(red+f , green,  blue);
    glVertex3f(red+f , green,  blue+f);

    // Left Face
    glVertex3f(red, green+f ,  blue+f);
    glVertex3f(red, green+f ,  blue);
    glVertex3f(red, green,  blue);
    glVertex3f(red, green,  blue+f);

    glEnd();

    //WIREFRAME AROUND CUBE
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
    glColor4f(red / 25.5f, green / 25.5f, blue / 25.5f, 0.7f);

    // Front Face
    glVertex3f(red, green, blue + f);
    glVertex3f(red, green + f, blue + f);
    glVertex3f(red + f, green + f, blue + f);
    glVertex3f(red + f, green, blue + f);

    // Back Face
    glVertex3f(red, green, blue);
    glVertex3f(red, green + f, blue);
    glVertex3f(red + f, green + f, blue);
    glVertex3f(red + f, green, blue);

    // Top Face
    glVertex3f(red, green + f, blue);
    glVertex3f(red, green + f, blue + f);
    glVertex3f(red + f, green + f, blue + f);
    glVertex3f(red + f, green + f, blue);
    // Bottom Face
    glVertex3f(red, green, blue);
    glVertex3f(red, green, blue + f);
    glVertex3f(red + f, green, blue + f);
    glVertex3f(red + f, green, blue);

    // Right face
    glVertex3f(red + f, green + f, blue + f);
    glVertex3f(red + f, green + f, blue);
    glVertex3f(red + f, green, blue);
    glVertex3f(red + f, green, blue + f);

    // Left Face
    glVertex3f(red, green + f, blue + f);
    glVertex3f(red, green + f, blue);
    glVertex3f(red, green, blue);
    glVertex3f(red, green, blue + f);

    glEnd();
}

void GLWidget::updateRGBCube() {
    drawingAxes();  //Zeichnen des äußeren Cubes
    if (roiMat.size > 0) {
        drawingCubes(); //Zeichnen der Pixel als Cubes
    }
}

/*********************************************************************************/

void GLWidget::getRoiCalcMap() {
    std::cout << "ROI loaded..." << std::endl;

    /**** hardcoded picture ****/
    //roiMat = cv::imread("C:\\Users\\adam\\Downloads\\RGB_9PIXEL_3SAME.bmp");
    //roiMat = cv::imread("C:\\Users\\adam\\Downloads\\RGB_3PIXEL.bmp");
    //roiMat = cv::imread("C:\\Users\\adam\\Downloads\\APM_2_5_MOTORS_QUAD_enc.jpg");

    computeRgbMap(roiMat);
    updateGL();
}



