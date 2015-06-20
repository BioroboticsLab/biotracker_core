#include "TextureObject.h"

TextureObject::TextureObject(QObject *parent)
    :QObject(parent)
{
    // OpenCV's coordinate system originates in the upper left corner.
    // OpenGL originates in the lower left. Thus the image has to be flipped vertically
    for (int j = 0; j < 4; ++j) {
        _texCoords.append(QVector2D(j == 0 || j == 3, j == 2 || j == 3));
    }
}

TextureObject::~TextureObject()
{

}

void TextureObject::draw() const
{
    //glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glVertexPointer(2, GL_FLOAT, 0, _vertices.constData());
    glTexCoordPointer(2, GL_FLOAT, 0, _texCoords.constData());
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_POLYGON, 0, 4);
    glDisable(GL_TEXTURE_2D);
    //glPopMatrix();
}

void TextureObject::createVertices(int cols, int rows)
{
    //create a vertice as wide and long as the image has pixels
    //for later being able to locate mouseclicks
    int corner1[2] = { cols, 0 };
    int corner2[2] = { 0, 0 };
    int corner3[2] = { 0, rows };
    int corner4[2] = { cols, rows };
    _vertices.clear();
    _vertices.append(QVector2D(static_cast<float>(corner1[0]), static_cast<float>(corner1[1])));
    _vertices.append(QVector2D(static_cast<float>(corner2[0]), static_cast<float>(corner2[1])));
    _vertices.append(QVector2D(static_cast<float>(corner3[0]), static_cast<float>(corner3[1])));
    _vertices.append(QVector2D(static_cast<float>(corner4[0]), static_cast<float>(corner4[1])));
}

void TextureObject::createTexture(cv::Mat picture)
{
    // free memory
    glDeleteTextures(1, &_texture);
    glVertexPointer(2, GL_FLOAT, 0, _vertices.constData());
    glTexCoordPointer(2, GL_FLOAT, 0, _texCoords.constData());
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // Non-mipmap way of mapping the texture (fast and clean):		// Allocate the texture
    glGenTextures(1, &_texture);
    // Select the texture.
    glBindTexture(GL_TEXTURE_2D, _texture);
    // If texture area is larger then the image, upscale using no interpolation.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // If texture area is smaller than the image, downsample using no interpolation.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // create Texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picture.cols, picture.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, picture.data);
}

void TextureObject::setPicture(cv::Mat picture)
{
    if( picture.cols != _cols || picture.rows != _rows)
    {
        _cols = picture.cols; _rows = picture.rows;
        createVertices(_cols, _rows);
    }
    createTexture(picture);
}

