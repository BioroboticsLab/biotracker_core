/*	
@author: Tobias von Falkenhausen
*/

#include "VideoView.h"
#include <QMouseEvent>

// OS X puts the headers in a different location in the include path than
// Windows and Linux, so we need to distinguish between OS X and the other
// systems.
#ifdef __APPLE__
	#include <OpenGL/glu.h>
#else
	#include <GL/glu.h>
#endif

QMutex trackMutex;

VideoView::VideoView(QWidget *parent)
	: QGLWidget(parent),
	_tracker(nullptr)
{
	_zoomFactor = 1.0;
	_panX = 0;
	_panY = 0;
	_isPanZoomMode = false;
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
	float imgRatio = static_cast<float>(_displayImage.cols) / _displayImage.rows;
	float windowRatio = static_cast<float>(width) / height;
	if(windowRatio < imgRatio) 
	{
		_zoomFactor = _displayImage.rows/(width/imgRatio);
		_panY = -((height - (width/imgRatio))/2)*_zoomFactor;
		_panX = 0;
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
		try
		{
			QMutexLocker locker(&trackMutex);
			_tracker->paint(imageCopy);
		}
		catch(std::exception&)
		{
			emit notifyGUI("critical error in selected tracking algorithm's paint method!",MSGS::FAIL);
		}

	}

	// Create a black background for the parts of the widget with no image.
	qglClearColor(Qt::black); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	int corner1[2] = {_displayImage.cols,0};
	int corner2[2] = {0,0};
	int corner3[2] = {0,_displayImage.rows};
	int corner4[2] = {_displayImage.cols,_displayImage.rows};
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

	// check window resolution and scale image if window resolution is lower than image resolution
	if (_zoomFactor > 1)
	{	
		QMutexLocker locker(&trackMutex);
		cv::resize(imageCopy,imageCopy,cv::Size(imageCopy.cols/_zoomFactor,imageCopy.rows/_zoomFactor),cv::INTER_AREA);//resize image
	}	

	/**
	* FOR PERFORMANCE ONLY LOAD VISIBLE PARTS OF THE PICTURE INTO GRAPHICS MEMORY
	*/
	// create Texture Atlas
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageCopy.cols, imageCopy.rows, 0, GL_BGR, GL_UNSIGNED_BYTE,0);// imageCopy.data);

	// variables for splitting image into tiles:
	// c = starting column, r = starting row
	// width	= width  of visible part of picture
	// height	= height	- " -		- " -
	int c=0,r=0;
	int width=_displayImage.cols;
	int height=_displayImage.rows;
	//Tile size
	int N = 64;
	QPoint lowerRight = unprojectMousePos(QPoint(this->width(),this->height()));
	QPoint upperLeft = unprojectMousePos(QPoint(0,0));
	if(upperLeft.x() > 0 )
		c=upperLeft.x();
	if(upperLeft.y() > 0 )
		r=upperLeft.y();
	if(lowerRight.x() < width-1)
		width=lowerRight.x()+1;
	if(lowerRight.y() < height-1)
		height=lowerRight.y()+1;

	if (_zoomFactor > 1)
	{
		c = (c*imageCopy.cols)/_displayImage.cols;
		r = (r*imageCopy.rows)/_displayImage.rows;
		width = (width*imageCopy.cols)/_displayImage.cols;
		height = (height*imageCopy.rows)/_displayImage.rows;
	}

	//to avoid artifacts when using 'glTexSubImage2d' with opencv MAT data,
	//set pixel storage mode to byte alignment
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1 );
	//and define number of pixels in a row
	glPixelStorei(GL_UNPACK_ROW_LENGTH, static_cast<int>(imageCopy.step)/imageCopy.channels());
	cv::Mat tile = imageCopy(cv::Range(r, height),
		cv::Range(c, width));
	glTexSubImage2D(GL_TEXTURE_2D, 0, c, r, tile.cols, tile.rows, GL_BGR, GL_UNSIGNED_BYTE, tile.ptr());

	// Draw it!
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4); 
	// free memory		
	imageCopy.release();
	glDeleteTextures(1, &_texture);
}

void VideoView::initializeGL()
{
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


QPoint VideoView::unprojectMousePos(QPoint mouseCoord)
{
	//variables required to map window coordinates to picture coordinates 
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLdouble posX, posY, posZ;
	QPoint pictureCoord;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );
	GLint isOnPicture = gluUnProject(mouseCoord.x(), viewport[3] - mouseCoord.y(), 0, modelview, projection, viewport, &posX, &posY, &posZ);
	pictureCoord.setX(static_cast<int>(posX));
	pictureCoord.setY(static_cast<int>(posY));
	return pictureCoord;
}

void VideoView::setTrackingAlgorithm(TrackingAlgorithm &trackingAlgorithm)
{
	QMutexLocker locker(&trackMutex);	
	_tracker = &trackingAlgorithm;		
}

void VideoView::takeScreenshot(QString screenShotFilename)
{
	cv::imwrite(screenShotFilename.toStdString(),_displayImage);
}

void VideoView::mouseMoveEvent( QMouseEvent * e )
{
	if (_isPanZoomMode)
	{
		if(_isPanning)
		{
			int dX = e->x() - _lastMPos[0];
			int dY = e->y() - _lastMPos[1];
			_lastMPos[0] = e->x();
			_lastMPos[1] = e->y();
			_panX -= dX * _zoomFactor;
			_panY -= dY * _zoomFactor;
			resizeGL(this->width(), this->height());
			//Draw the scene
			updateGL();
		}
	}
	else
	{

		QPoint p  = unprojectMousePos(e->pos());
		const QPointF *localPos = new QPointF(p);
		QMouseEvent *modifiedEvent = new QMouseEvent(e->type(),*localPos,e->screenPos(),e->button(),e->buttons(),e->modifiers());			
		emit moveEvent ( modifiedEvent );		

	}
}

void VideoView::mousePressEvent( QMouseEvent * e )
{
	if (_isPanZoomMode)
	{
		if(QApplication::keyboardModifiers() == Qt::NoModifier)
		{
			_isPanning = true;
			this->setCursor(Qt::ClosedHandCursor);
			_lastMPos[0] = e->x();
			_lastMPos[1] = e->y();		
		}
		if(e->button() == Qt::LeftButton && e->type() == QEvent::MouseButtonDblClick)
		{
			fitToWindow();
		}
	}
	else
	{
		QPoint p  = unprojectMousePos(e->pos());
		const QPointF *localPos = new QPointF(p);
		QMouseEvent *modifiedEvent = new QMouseEvent(e->type(),*localPos,e->screenPos(),e->button(),e->buttons(),e->modifiers());	
		emit pressEvent ( modifiedEvent );
	}
}

void VideoView::mouseReleaseEvent( QMouseEvent * e )
{
	if (_isPanZoomMode)
	{
		this->setCursor(Qt::OpenHandCursor);
		_isPanning = false;
	}
	else{
		QPoint p  = unprojectMousePos(e->pos());
		const QPointF *localPos = new QPointF(p);
		QMouseEvent *modifiedEvent = new QMouseEvent(e->type(),*localPos,e->screenPos(),e->button(),e->buttons(),e->modifiers());	
		emit releaseEvent ( modifiedEvent );
	}
}

void VideoView::wheelEvent( QWheelEvent * e )
{
	QPoint picturePos  = unprojectMousePos(e->pos());

	if (_isPanZoomMode)
	{
		int numDegrees = e->delta();
		if (e->orientation() == Qt::Vertical
			&& _zoomFactor - 0.001 * numDegrees > 0) 
		{
			_zoomFactor -= 0.001 * numDegrees;
			// adjust _panX and _panY, so that zoom is centered on mouse cursor
			if (picturePos.x() > 0 && picturePos.x() < _displayImage.cols && this->width() < _displayImage.cols/_zoomFactor )
				_panX =		picturePos.x()-  ((this->width()*_zoomFactor)/2);
			else
				_panX = _panX/2;
			if(picturePos.y() > 0 && picturePos.y() < _displayImage.rows && this->height() < _displayImage.rows/_zoomFactor)
				_panY =		picturePos.y() - ((this->height()*_zoomFactor)/2);
			resizeGL(this->width(), this->height());
			//Draw the scene
			updateGL();
			e->accept();

		}
	}
	else
	{
		const QPointF *localPos = new QPointF(picturePos);
		QWheelEvent *modifiedEvent = new QWheelEvent(e->pos(),*localPos,e->pixelDelta(),e->angleDelta(),e->delta(),e->orientation(),e->buttons(),e->modifiers());
		emit mouseWheelEvent( modifiedEvent );
	}	
}

void VideoView::setPanZoomMode(bool isPanZoom)
{
	_isPanZoomMode = isPanZoom;
	if(_isPanZoomMode)
		this->setCursor(Qt::OpenHandCursor);
	else
		this->setCursor(Qt::ArrowCursor);
}
cv::Mat VideoView::getCurrentScreen()
{
	return _displayImage.clone();
}