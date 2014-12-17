/*	
@author: Tobias von Falkenhausen
*/

#include "VideoView.h"
#include <QMouseEvent>

#include "source/tracking/TrackingAlgorithm.h"

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
	: QGLWidget(parent)
	, _tracker(nullptr)
	, _isPanZoomMode(false)
	, _currentWidth(0)
	, _currentHeight(0)
	, _zoomFactor(0)
	, _screenPicRatio(0)
	, _panX(0)
	, _panY(0)
	, _isPanning(false)
	, _lastPannedTime(std::chrono::system_clock::now())
	, _lastZoomedTime(std::chrono::system_clock::now())
	, _lastZoomedPoint(0, 0)
{}

void VideoView::showImage(cv::Mat img)
{
	_displayImage = img;
	resizeGL(width(), height());


	//Draw the scene
	updateGL();
}

void VideoView::fitToWindow()
{
	if (!_displayImage.empty()) {
		_zoomFactor = 0;
		float width = static_cast<float>(this->width());
		float height = static_cast<float>(this->height());
		float imgRatio = static_cast<float>(_displayImage.cols) / _displayImage.rows;
		float windowRatio = static_cast<float>(width) / height;
		if(windowRatio < imgRatio)
		{
			_panY = -((height - (width/imgRatio))/2)*(_screenPicRatio + _zoomFactor);
			_panX = 0;
		} else
		{
			_panX = - ((width - (height*imgRatio))/2)*(_screenPicRatio +_zoomFactor);
			_panY = 0;
		}
		glViewport(0,0,this->width(), this->height());
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		width = width * (_screenPicRatio + _zoomFactor);
		height = height *(_screenPicRatio + _zoomFactor);

		float left = _panX;
		float top	 = _panY;
		float right = left + width;
		float bottom = top + height;
		glOrtho(left, right, bottom, top, 0.0, 1.0);
		glMatrixMode(GL_MODELVIEW);
		//resizeGL(width, height);
		//Draw the scene
		updateGL();
	}
}

void VideoView::paintGL()
{
	// Create a black background for the parts of the widget with no image.
	qglClearColor(Qt::black);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
		catch(std::exception& err)
		{
			std::stringstream ss;
			ss << "critical error in selected tracking algorithm's paint method!";
			ss << "\n" << err.what();
			emit notifyGUI(ss.str() ,MSGS::FAIL);
		}

	}

	glLoadIdentity();

	int corner1[2] = {_displayImage.cols,0};
	int corner2[2] = {0,0};
	int corner3[2] = {0,_displayImage.rows};
	int corner4[2] = {_displayImage.cols,_displayImage.rows};
	_vertices.clear();
	_vertices.append(QVector2D(static_cast<float>(corner1[0]), static_cast<float>(corner1[1])));
	_vertices.append(QVector2D(static_cast<float>(corner2[0]), static_cast<float>(corner2[1])));
	_vertices.append(QVector2D(static_cast<float>(corner3[0]), static_cast<float>(corner3[1])));
	_vertices.append(QVector2D(static_cast<float>(corner4[0]), static_cast<float>(corner4[1])));

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
	if ((_screenPicRatio + _zoomFactor) > 1)
	{	
		QMutexLocker locker(&trackMutex);
		cv::resize(imageCopy, imageCopy, cv::Size(static_cast<int>(imageCopy.cols / (_screenPicRatio + _zoomFactor)),
			static_cast<int>(imageCopy.rows / (_screenPicRatio + _zoomFactor))), cv::INTER_AREA);//resize image
	}	

	/**
	* FOR PERFORMANCE LOAD JUST THE VISIBLE PARTS OF THE PICTURE INTO GRAPHICS MEMORY
	*/
	// create Texture Atlas
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageCopy.cols, imageCopy.rows, 0, GL_BGR, GL_UNSIGNED_BYTE,0);// imageCopy.data);

	//check which part of the picture is on screen
	//by unprojecting lower right and upper left corner
	QPoint lowerRight = unprojectScreenPos(QPoint(this->width(),this->height()));
	QPoint upperLeft = unprojectScreenPos(QPoint(0,0));
   
	//if image was dragged out abort painting
	if (upperLeft.x() > _displayImage.cols || upperLeft.y() > _displayImage.rows || lowerRight.x() < 0 || lowerRight.y() < 0)
		return;
	//otherwise set variables indicating which part of picture is visible
	int c=0,r=0;
	int width=_displayImage.cols;
	int height=_displayImage.rows;
	if(upperLeft.x() > 0 )
		c=upperLeft.x();
	if(upperLeft.y() > 0 )
		r=upperLeft.y();
	if(lowerRight.x() < width-1)
		width=lowerRight.x()+1;
	if(lowerRight.y() < height-1)
		height=lowerRight.y()+1;

	//if image was scaled down previously 
	//we need to adjust coordinates relatively
	if ((_screenPicRatio + _zoomFactor) > 1)
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
	glDrawArrays(GL_POLYGON, 0, 4);
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
	// dont do anything if  width or height are 0 
	// or there is no image to display
	if (width <= 0 || height <= 0)
		return;
	if (_displayImage.empty())
		return;

	//calculate ratio of screen to displayed image
	float imgRatio = static_cast<float>(_displayImage.cols) / _displayImage.rows;
	float windowRatio = static_cast<float>(width) / height;
	if(windowRatio < imgRatio)
	{
		_screenPicRatio = _displayImage.rows/(width/imgRatio);		
	}
	else
	{
		_screenPicRatio = _displayImage.cols/(height*imgRatio);		
	}

	glViewport(0,0,width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//if window really was resized,
	//fit it to window at end of this function
	bool sizeChanged = false;
	if(_currentHeight != height || _currentWidth != width)
	{
		sizeChanged = true;
		_currentHeight = height;
		_currentWidth = width;
	}

	width = static_cast<int>(width * (_screenPicRatio + _zoomFactor));
	height = static_cast<int>(height *(_screenPicRatio + _zoomFactor));

	float left = _panX;
	float top	 = _panY;
	float right = left + width;
	float bottom = top + height;
	glOrtho(left, right, bottom, top, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);	
	if (sizeChanged)
		fitToWindow();
	emit reportZoomLevel(_zoomFactor);
}


QPoint VideoView::unprojectScreenPos(QPoint position)
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
	/*GLint isOnPicture = */ gluUnProject(position.x(), viewport[3] - position.y(), 0, modelview, projection, viewport, &posX, &posY, &posZ);
	pictureCoord.setX(static_cast<int>(posX));
	pictureCoord.setY(static_cast<int>(posY));
	return pictureCoord;
}

void VideoView::setTrackingAlgorithm(std::shared_ptr<TrackingAlgorithm> trackingAlgorithm)
{
	QMutexLocker locker(&trackMutex);	
	_tracker = trackingAlgorithm;		
}

void VideoView::takeScreenshot(QString screenShotFilename)
{
	cv::imwrite(screenShotFilename.toStdString(),_displayImage);
}

void VideoView::keyPressEvent(QKeyEvent *e)
{
	e->accept();
	QKeyEvent event(e->type(), e->key(), e->modifiers(), e->text());
	QCoreApplication::sendEvent(QApplication::activeWindow(), &event);
}

void VideoView::mouseMoveEvent( QMouseEvent * e )
{
	if (_isPanZoomMode)
	{
		const auto elapsed = std::chrono::system_clock::now() - _lastPannedTime;
		if (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() > 1) {
			if(_isPanning)
			{
				int dX = e->x() - _lastMPos[0];
				int dY = e->y() - _lastMPos[1];
				_lastMPos[0] = e->x();
				_lastMPos[1] = e->y();
				_panX -= dX * (_screenPicRatio+_zoomFactor);
				_panY -= dY * (_screenPicRatio+_zoomFactor);
				resizeGL(this->width(), this->height());
				//Draw the scene
				updateGL();
			}
			_lastPannedTime = std::chrono::system_clock::now();
		}
	}
	else
	{
		e->accept();
		QPoint p  = unprojectScreenPos(e->pos());
		const QPointF localPos(p);
		QMouseEvent modifiedEvent(e->type(),localPos,e->screenPos(),e->button(),e->buttons(),e->modifiers());
		QCoreApplication::sendEvent(QApplication::activeWindow(), &modifiedEvent);
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
		e->accept();
		QPoint p  = unprojectScreenPos(e->pos());
		const QPointF localPos(p);
		QMouseEvent modifiedEvent(e->type(),localPos,e->screenPos(),e->button(),e->buttons(),e->modifiers());
		QCoreApplication::sendEvent(QApplication::activeWindow(), &modifiedEvent);
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
		e->accept();
		QPoint p  = unprojectScreenPos(e->pos());
		const QPointF localPos(p);
		QMouseEvent modifiedEvent(e->type(),localPos,e->screenPos(),e->button(),e->buttons(),e->modifiers());
		QCoreApplication::sendEvent(QApplication::activeWindow(), &modifiedEvent);
	}
}

void VideoView::wheelEvent( QWheelEvent * e )
{
	QPoint picturePos  = unprojectScreenPos(e->pos());

	if (_isPanZoomMode)
	{
		int numDegrees = e->delta();
		if (e->orientation() == Qt::Vertical
			&& (_zoomFactor+_screenPicRatio) - 0.002 * numDegrees > 0)
		{
			_zoomFactor -= 0.002f * numDegrees;

			auto elapsed = std::chrono::system_clock::now() - _lastZoomedTime;
			//when we zoomed only recently, center zoom to same spot again
			if (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() < 500)
			{
				_panX = _lastZoomedPoint.x() - ((this->width()*(_screenPicRatio + _zoomFactor)) / 2);
				_panY = _lastZoomedPoint.y() - ((this->height()*(_screenPicRatio + _zoomFactor)) / 2);
			}
			else
			{ 			
				// adjust _panX and _panY, so that zoom is centered on mouse cursor
				_panX = picturePos.x() - ((this->width()*(_screenPicRatio + _zoomFactor)) / 2);				
				_panY = picturePos.y() - ((this->height()*(_screenPicRatio + _zoomFactor)) / 2);
				_lastZoomedPoint = picturePos;
			}
			_lastZoomedTime = std::chrono::system_clock::now();
			
			if (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() > 1) {
				resizeGL(this->width(), this->height());
				//Draw the scene
				updateGL();
				e->accept();
			}
		}		
	}
	else
	{
		e->accept();
		QPoint p  = unprojectScreenPos(e->pos());
		const QPointF localPos(p);
		QWheelEvent modifiedEvent(e->pos(),localPos,e->pixelDelta(),e->angleDelta(),e->delta(),e->orientation(),e->buttons(),e->modifiers());
		QCoreApplication::sendEvent(QApplication::activeWindow(), &modifiedEvent);
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
