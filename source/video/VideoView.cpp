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
}

void VideoView::showImage(cv::Mat img)
{
	_displayImage = img;
	resizeGL(this->width(), this->height());


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

	glVertexPointer(2, GL_FLOAT, 0, _vertices.constData());
	glTexCoordPointer(2, GL_FLOAT, 0, _texCoords.constData());
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);



	// Non-mipmap way of mapping the texture (fast and clean):
	// Allocate a texture name
	glGenTextures(1, &_texture);
	// Select our current texture.
	glBindTexture(GL_TEXTURE_2D, _texture); 
	// When the texture area is larger then the image, upscale using linear interpolation.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// When the texture area is smaller than the image, downsample using linear interpolation.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	// wrap image onto the texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imageCopy.cols, imageCopy.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, imageCopy.data);

	//adjust zoom level | 1.0 = no zooming
	glScalef(_zoomFactor, _zoomFactor, 1.0);


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
		_vertices.append(QVector2D(coords[j][0], coords[j][1]));
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
	if(windowRatio < imgRatio) 
	{
		glViewport(0, (height-width/imgRatio)/2, width, width/imgRatio);
	} else 
	{
		glViewport((width-height*imgRatio)/2, 0, height*imgRatio, height);
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-1.0, +1.0, +1.0, -1.0, 0.0, 1.0);
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
	emit moveEvent ( e );
}

void VideoView::mousePressEvent( QMouseEvent * e )
{
	emit pressEvent ( e );
}

void VideoView::mouseReleaseEvent( QMouseEvent * e )
{
	emit releaseEvent ( e );
}

void VideoView::wheelEvent( QWheelEvent * e )
{
	int numDegrees = e->delta();
	if (e->orientation() == Qt::Vertical && _zoomFactor + 0.001 * numDegrees > 0) {
		_zoomFactor += 0.001 * numDegrees;
		updateGL();
	}
	e->accept();
}