/*
@author: Tobias von Falkenhausen
*/

#include "VideoView.h"
#include <QMouseEvent>
#include <QGLFormat>
#include "source/tracking/TrackingAlgorithm.h"
#include <QMatrix>

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
    : QGLWidget(/*QGLFormat(QGL::SampleBuffers),*/parent)
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
	, _selectedView(TrackingAlgorithm::OriginalView)
{
    setAutoFillBackground(false);
}

VideoView::~VideoView(){}

void VideoView::showImage(cv::Mat img)
{
    _displayImage = img;
//    makeCurrent();
    _textureObj->setPicture(img);
    resizeGL(this->width(), this->height());

//    //Draw the scene
    update();
}

void VideoView::fitToWindow()
{
    makeCurrent();

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
        glViewport(0,0, static_cast<GLsizei>(width) ,static_cast<GLsizei>(height));
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

        emit reportZoomLevel(_screenPicRatio + _zoomFactor);

        //Draw the scene
        update();

    }
}

void VideoView::changeSelectedView(TrackingAlgorithm::View const& selectedView)
{
	_selectedView = selectedView;
    update();
}

//use paintEvent instad now!!
/*void VideoView::paintGL()
{
	makeCurrent();
	// Create a black background for the parts of the widget with no image.
	qglClearColor(Qt::black);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(_displayImage.empty()) 
	{
		// Don't bother painting an image if we have none.
		return; 
	}
	
	// if tracking algorithm is selected and zoom/pan is not active:
	if(_tracker && !_isPanZoomMode)
	{
		try
		{
			// create copy of curent image and send it for further drawing to tracking algorithm
			cv::Mat imageCopy = _displayImage.clone();
			QMutexLocker locker(&trackMutex);
            _tracker->paint(imageCopy, _selectedView); 
			// create new texture with processed image copy
			createTexture(imageCopy);
			imageCopy.release();
		}
		catch(std::exception& err)
		{
			std::stringstream ss;
			ss << "critical error in selected tracking algorithm's paint method!";
			ss << "\n" << err.what();
			emit notifyGUI(ss.str() ,MSGS::FAIL);
		}

	}
	// Draw it!
	glDrawArrays(GL_POLYGON, 0, 4);
}*/

void VideoView::paintEvent(QPaintEvent * /*event*/)
{
    if(_displayImage.empty())
    {
        // Don't bother painting an image if we have none.
        return;
    }
    makeCurrent();
    QPainter painter(this);


    if(_tracker)
    {
        try
        {
//            cv::Mat imageCopy = _displayImage.clone();
//            QMutexLocker locker(&trackMutex);
//            _tracker->paint(imageCopy, _selectedView);
//            _textureObj->setPicture(imageCopy);
//            imageCopy.release();
            ProxyPaintObject proxy (*this);
            _tracker->paint(proxy, _selectedView);
            if(!proxy._img.empty())
            {
                _textureObj->setPicture(proxy._img);
                proxy._img.release();
            }
        }
        catch(std::exception& err)
        {
            std::stringstream ss;
            ss << "critical error in selected tracking algorithm's paint method!";
            ss << "\n" << err.what();
            emit notifyGUI(ss.str() ,MSGS::FAIL);
        }        
    }    
    glMatrixMode(GL_MODELVIEW);
    //glPushMatrix();
    // Create a black background for the parts of the widget with no image.
    qglClearColor(Qt::black);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    _textureObj->draw();
    //glPopMatrix();

    if(_tracker)
    {
        painter.setWindow(QRect(0, 0, _displayImage.cols, _displayImage.rows));
        QPoint upperLeft = projectPicturePos(QPoint(0,0));
        QPoint lowerRight = projectPicturePos((QPoint(_displayImage.cols,_displayImage.rows)));
        int width = lowerRight.x() - upperLeft.x();
        int height = lowerRight.y() - upperLeft.y();
        painter.setViewport(upperLeft.x(), upperLeft.y(), width, height);
        _tracker->paintOverlay( &painter);
    }
    painter.end();
}



/*void VideoView::paintGL(){
    glDrawArrays(GL_POLYGON, 0, 4);
}*/

void VideoView::initializeGL()
{
    glEnable(GL_MULTISAMPLE);
    //glEnable(GL_TEXTURE_2D);
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    _textureObj = std::make_unique<TextureObject>(this);
    _textureObj->init();
}

void VideoView::resizeGL(int width, int height)
{
    //qreal pixelRatio = devicePixelRatio();
	makeCurrent();

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

	//create viewport with coordinates matching picture size in pixels
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
	emit reportZoomLevel(_screenPicRatio + _zoomFactor);
}

QPoint VideoView::unprojectScreenPos(QPoint position)
{
    makeCurrent();

	//variables required to map window coordinates to picture coordinates 
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLdouble posX, posY, posZ;
	QPoint pictureCoord;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );
    gluUnProject(position.x(), viewport[3] - position.y(), 0, modelview, projection, viewport, &posX, &posY, &posZ);
	pictureCoord.setX(static_cast<int>(posX));
	pictureCoord.setY(static_cast<int>(posY));
	return pictureCoord;
}

QPoint VideoView::projectPicturePos(QPoint position)
{
    makeCurrent();

    //variables required to map picture coordinates to window coordinates
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLdouble posX, posY, posZ;
    QPoint windowCoord;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
    gluProject(position.x(), position.y() , 0, modelview, projection, viewport, &posX, &posY, &posZ);
    windowCoord.setX(static_cast<int>(posX));
    windowCoord.setY(-(static_cast<int>(posY - viewport[3])));
    return windowCoord;
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
                update();
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
                update();
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
