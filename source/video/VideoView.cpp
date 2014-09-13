/*	
@author: Tobias von Falkenhausen
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
	int width = this->width();
	int height = this->height();
	float imgRatio = (float)_displayImage.cols/(float)_displayImage.rows;
	float windowRatio = (float)width/(float)height;
	if(windowRatio < imgRatio) 
	{
		_zoomFactor = _displayImage.rows/(width/imgRatio);
		_panY = -((height - (width/imgRatio))/2)*_zoomFactor;
		_panX = 0;
		emit notifyGUI("panY: " + StringHelper::iToSS(_panY),MSGS::NOTIFICATION);
	} else 
	{
		_zoomFactor = _displayImage.cols/(height*imgRatio);
		_panX = - ((width - (height*imgRatio))/2)*_zoomFactor;	
		_panY = 0;
	}
	resizeGL(width, height);
	//Draw the scene
	updateGL();
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

	int corner1[2] = {imageCopy.cols,0};
	int corner2[2] = {0,0};
	int corner3[2] = {0,imageCopy.rows};
	int corner4[2] = {imageCopy.cols,imageCopy.rows};
	_vertices.clear();
	_vertices.append(QVector2D(corner1[0],corner1[1]));
	_vertices.append(QVector2D(corner2[0],corner2[1]));
	_vertices.append(QVector2D(corner3[0],corner3[1]));
	_vertices.append(QVector2D(corner4[0],corner4[1]));

	glVertexPointer(2, GL_FLOAT, 0, _vertices.constData());
	glTexCoordPointer(2, GL_FLOAT, 0, _texCoords.constData());
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);



	// Non-mipmap way of mapping the texture (fast and clean):
	// Allocate the texture
	glGenTextures(1, &_texture);
	// Select the texture.
	glBindTexture(GL_TEXTURE_2D, _texture); 
	// If texture area is larger then the image, upscale using no interpolation.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// If texture area is smaller than the image, downsample using no interpolation.
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
			
	// OpenCV's coordinate system originates in the upper left corner.
	// OpenGL originates in the lower left. Thus the image has to be flipped vertically		
	for (int j = 0; j < 4; ++j) {
		_texCoords.append(QVector2D(j == 0 || j == 3, j == 2 || j == 3));	
	}
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);	
}
void VideoView::resizeGL(int width, int height)
{
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
	//init variables required to map window coordinates to picture coordinates 
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
	if(e->button() == Qt::LeftButton && e->type() == QEvent::MouseButtonDblClick)
	{
		fitToWindow();
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