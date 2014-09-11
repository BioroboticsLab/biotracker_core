/*	
Copyright(c) 2010 Johannes Jordan <johannes.jordan@cs.fau.de>.

This file may be licensed under the terms of of the GNU General Public
License, version 3, as published by the Free Software Foundation. You can
find it here: http://www.gnu.org/licenses/gpl.html

changed by Tobias von Falkenhausen
*/

#include "VideoView.h"
#include <QMouseEvent>
#include <gl\GLU.h>

QMutex trackMutex;

VideoView::VideoView(QWidget *parent)
	: QGLWidget(parent),
	_tracker(NULL)
{
	_zoomFactor = 1.0;
	_panX = 0;
	_panY = 0;
}

void VideoView::showImage(cv::Mat img)
{
	_displayImage = img;
	resizeGL(this->width(), this->height());


	//Draw the scene
	updateGL();
}

void VideoView::fitToWindow()
{
	//int width = this->width();
	//int height = this->height();
	//float imgRatio = (float)_displayImage.cols/(float)_displayImage.rows;
	//float windowRatio = (float)width/(float)height;
	//if(windowRatio < imgRatio) 
	//{
	//	emit notifyGUI("windowHeight: " + StringHelper::iToSS(this->height()) + " videoHeight(original): " 
	//		+ StringHelper::iToSS(_displayImage.rows) + " videoHeight(adapted): " + StringHelper::iToSS(width/imgRatio),MSGS::NOTIFICATION);
	//	_zoomFactor = ((width/imgRatio)/_displayImage.rows);
	//	_panY = (height - (width/imgRatio))/2;
	//} else 
	//{
	//	emit notifyGUI("windowWidth: " + StringHelper::iToSS(this->width()) + " videoWidth(original): " 
	//		+ StringHelper::iToSS(_displayImage.cols) + " videoWidth(adapted): " + StringHelper::iToSS(height*imgRatio),MSGS::NOTIFICATION);
	//	_zoomFactor = (width/imgRatio)/_displayImage.rows;
	//	_panX = (width - (height*imgRatio))/2;		
	//}
	//resizeGL(this->width(), this->height());
	////Draw the scene
	//updateGL();
	//std::ostringstream ss;
	//ss << _zoomFactor;
	//std::string s(ss.str());
	//emit notifyGUI("zoom factor" + s,MSGS::NOTIFICATION);
}

void VideoView::paintGL()
{

	if(_displayImage.empty()) 
	{
		// Don't bother painting an image if we have none.
		return; 
	}
	cv::Mat imageCopy = _displayImage.clone();
	if(_tracker)
	{
		QMutexLocker locker(&trackMutex);
		_tracker->paint(imageCopy);
	}

	// Create a black background for the parts of the widget with no image.
	qglClearColor(Qt::black); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glVertexPointer(2, GL_FLOAT, 0, _vertices.constData());
	glTexCoordPointer(2, GL_FLOAT, 0, _texCoords.constData());
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	int ro[2] = {imageCopy.cols,0};
	int lu[2] = {0,0};
	int ru[2] = {0,imageCopy.rows};
	int lo[2] = {imageCopy.cols,imageCopy.rows};
	_vertices.clear();
	_vertices.append(QVector2D(ro[0],ro[1]));
	_vertices.append(QVector2D(lu[0],lu[1]));
	_vertices.append(QVector2D(ru[0],ru[1]));
	_vertices.append(QVector2D(lo[0],lo[1]));

	// Non-mipmap way of mapping the texture (fast and clean):
	// Allocate a texture name
	glGenTextures(1, &_texture);
	// Select our current texture.
	glBindTexture(GL_TEXTURE_2D, _texture); 
	// When the texture area is larger then the image, upscale using no interpolation.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// When the texture area is smaller than the image, downsample using no interpolation.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	// wrap image onto the texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imageCopy.cols, imageCopy.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, imageCopy.data);

	// Draw it!
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4); 

	// free memory
	imageCopy.release();
	glDeleteTextures(1, &_texture);
}

void VideoView::initializeGL()
{
	// Create the surface we will use for the texture:
	static const int coords[4][3] = { { +1, -1 }, { -1, -1 }, { -1, +1 }, { +1, +1 } };
	for (int j = 0; j < 4; ++j) {
		/* A note about texture coordinates:
		OpenCV uses a nice, sane coordinate system with origin in the upper left corner.
		Just like any other image processing tool (let's just forget the fact that math-wise
		that is silly).
		OpenGL, however, uses a math-inspired coordinate system with origin in the lower
		left.
		Right here, the texture is mapped, so the image is automatically flipped in the y-
		direction. Better do it here, than actually flipping the image elsewhere.
		*/
		_texCoords.append(QVector2D(j == 0 || j == 3, j == 2 || j == 3));
		//_vertices.append(QVector2D(coords[j][0], coords[j][1]));
	}
	

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
}
void VideoView::resizeGL(int width, int height)
{
	// Make sure the image keeps its aspect ratio, regardless of widget size:
	// (also, center it in the widget)
	float imgRatio = (float)_displayImage.cols/(float)_displayImage.rows;
	float windowRatio = (float)width/(float)height;
	//if(windowRatio < imgRatio) 
	//{
	//	glViewport(0, (height-width/imgRatio)/2, width, width/imgRatio);
	//} else 
	//{
	//	glViewport((width-height*imgRatio)/2, 0, height*imgRatio, height);
	//}
	glViewport(0,0,width, height);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	width = width * _zoomFactor;
	height = height * _zoomFactor;

	float left = _panX;
	float top	 = _panY;
	float right = left + width;
	float bottom = top + height;
	glOrtho(left, right, bottom, top, 0.0, 1.0);
	

	//glOrtho(-1.0, +1.0, +1.0, -1.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}


void VideoView::setTrackingAlgorithm(TrackingAlgorithm &trackingAlgorithm)
{
	QMutexLocker locker(&trackMutex);	
	_tracker = &trackingAlgorithm;		
}

void VideoView::takeScreenshot(QString screenShotFilename)
{
	cv::imwrite(StringHelper::toStdString(screenShotFilename),_displayImage);
}

void VideoView::mouseMoveEvent( QMouseEvent * e )
{
	if(_isPanning)
	{
		int dX = e->x() - _lastMPos[0];
		int dY = e->y() - _lastMPos[1];
		_lastMPos[0] = e->x();
		_lastMPos[1] = e->y();
		_panX -= dX;
		_panY -= dY;
		resizeGL(this->width(), this->height());
		//Draw the scene
		updateGL();
	}
	//variables required to map window coordinates to picture coordinates 
	GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
	GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );
	GLint isOnPicture = gluUnProject(e->x(), viewport[3] - e->y(), 0, modelview, projection, viewport, &posX, &posY, &posZ);
	if(isOnPicture)
	{	QPoint p  ((int)posX, (int)posY);
		const QPointF *localPos = new QPointF(p);
		QMouseEvent *modifiedEvent = new QMouseEvent(e->type(),*localPos,e->screenPos(),e->button(),e->buttons(),e->modifiers());
		emit moveEvent ( modifiedEvent );		
	}
}

void VideoView::mousePressEvent( QMouseEvent * e )
{
	if(QApplication::keyboardModifiers() == Qt::NoModifier)
	{
		_isPanning = true;
		_lastMPos[0] = e->x();
		_lastMPos[1] = e->y();		
	}
		//variables required to map window coordinates to picture coordinates 
	GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
	GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );
	GLint isOnPicture = gluUnProject(e->x(), viewport[3] - e->y(), 0, modelview, projection, viewport, &posX, &posY, &posZ);
	if(isOnPicture)
	{
		QPoint p  ((int)posX, (int)posY);
		const QPointF *localPos = new QPointF(p);
		QMouseEvent *modifiedEvent = new QMouseEvent(e->type(),*localPos,e->screenPos(),e->button(),e->buttons(),e->modifiers());
		emit pressEvent ( modifiedEvent );
	}
}

void VideoView::mouseReleaseEvent( QMouseEvent * e )
{
	_isPanning = false;
	
	//variables required to map window coordinates to picture coordinates 
	GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
	GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );
	GLint isOnPicture = gluUnProject(e->x(), viewport[3] - e->y(), 0, modelview, projection, viewport, &posX, &posY, &posZ);
	if(isOnPicture)
	{
		QPoint p  ((int)posX, (int)posY);
		const QPointF *localPos = new QPointF(p);
		QMouseEvent *modifiedEvent = new QMouseEvent(e->type(),*localPos,e->screenPos(),e->button(),e->buttons(),e->modifiers());
		emit releaseEvent ( modifiedEvent );
	}
}

void VideoView::wheelEvent( QWheelEvent * e )
{
	int numDegrees = e->delta();
	if (e->orientation() == Qt::Vertical && _zoomFactor + 0.001 * numDegrees > 0) {
		_zoomFactor += 0.001 * numDegrees;
		resizeGL(this->width(), this->height());
		//Draw the scene
		updateGL();
	}
	e->accept();
}