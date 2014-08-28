/*	
Copyright(c) 2010 Johannes Jordan <johannes.jordan@cs.fau.de>.

This file may be licensed under the terms of of the GNU General Public
License, version 3, as published by the Free Software Foundation. You can
find it here: http://www.gnu.org/licenses/gpl.html

changed by Tobias von Falkenhausen
*/

#include "VideoView.h"

VideoView::VideoView(QWidget *parent)
	: QGLWidget(parent)
{
}

void VideoView::showImage(cv::Mat img)
{
	_displayImage = img;
	resizeGL(this->width(), this->height());


	//Draw the scene
	update();
}


void VideoView::paintGL()
{
	if(_displayImage.empty()) {
		return; // Don't bother painting an image if we have none.
	}

	qglClearColor(Qt::black); // Create a nice, black background for the parts of the widget with no image.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glVertexPointer(2, GL_FLOAT, 0, _vertices.constData());
	glTexCoordPointer(2, GL_FLOAT, 0, _texCoords.constData());
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	
	// Non-mipmap way of mapping the texture (fast and clean):
	glGenTextures(1, &_texture); // Allocate a texture name.
	glBindTexture(GL_TEXTURE_2D, _texture); // Select our current texture.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // When the texture area is larger then the image, upscale using linear interpolation.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // When the texture area is smaller than the image, downsample using linear interpolation.
	glTexImage2D(GL_TEXTURE_2D, 0, 3, _displayImage.cols, _displayImage.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, _displayImage.data);

	// End of different methods, the last few lines are common for all methods.

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // Draw it!

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
	if(windowRatio < imgRatio) {
		glViewport(0, (height-width/imgRatio)/2, width, width/imgRatio);
	} else {
		glViewport((width-height*imgRatio)/2, 0, height*imgRatio, height);
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, +1.0, +1.0, -1.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void VideoView::takeScreenshot(QString screenShotFilename)
{
	cv::imwrite(StringHelper::toStdString(screenShotFilename),_displayImage);
}

void VideoView::mousePressEvent(QMouseEvent *e)
{
	switch (e->button())
	{
		case Qt::LeftButton:       
			emit mousePressEventL(QVector2D(e->pos()));			
			break;
		case Qt::RightButton:
			emit mousePressEventR(QVector2D(e->pos()));
			break;
		case Qt::MiddleButton:      
			emit mousePressEventM(QVector2D(e->pos()));
			break;
		default: 
			break;
	}
}

void VideoView::mouseReleaseEvent(QMouseEvent *e)
{
	switch (e->button())
	{
		case Qt::LeftButton:       
			emit mouseReleaseEventL(QVector2D(e->pos()));			
			break;
		case Qt::RightButton:
			emit mouseReleaseEventR(QVector2D(e->pos()));
			break;
		case Qt::MiddleButton:      
			emit mouseReleaseEventM(QVector2D(e->pos()));
			break;
		default: 
			break;
	}
}