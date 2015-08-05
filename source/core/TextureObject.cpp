#include "TextureObject.h"

namespace BioTracker{
namespace Core {

TextureObject::TextureObject(QObject *parent, QOpenGLContext *context)
    : //QObject(parent)
    // ,
        m_context(context)
{
    // OpenCV's coordinate system originates in the upper left corner.
    // OpenGL originates in the lower left. Thus the image has to be flipped vertically
    for (int j = 0; j < 4; ++j) {
        m_texCoords.append(QVector2D(j == 0 || j == 3, j == 2 || j == 3));
    }

    setImage(cv::Mat::zeros(1, 1, CV_8UC3));
}

void TextureObject::draw() const
{
    //m_context->makeCurrent(surface);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glVertexPointer(2, GL_FLOAT, 0, m_vertices.constData());
    glTexCoordPointer(2, GL_FLOAT, 0, m_texCoords.constData());
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_POLYGON, 0, 4);
    glDisable(GL_TEXTURE_2D);
}

void TextureObject::createVertices()
{
    //create a vertice as wide and long as the image has pixels
    //for later being able to locate mouseclicks
    const int corner1[2] = { m_img.cols, 0 };
    const int corner2[2] = { 0, 0 };
    const int corner3[2] = { 0, m_img.rows };
    const int corner4[2] = { m_img.cols, m_img.rows };
    m_vertices.clear();
    m_vertices.append(QVector2D(static_cast<float>(corner1[0]), static_cast<float>(corner1[1])));
    m_vertices.append(QVector2D(static_cast<float>(corner2[0]), static_cast<float>(corner2[1])));
    m_vertices.append(QVector2D(static_cast<float>(corner3[0]), static_cast<float>(corner3[1])));
    m_vertices.append(QVector2D(static_cast<float>(corner4[0]), static_cast<float>(corner4[1])));
}

void TextureObject::createTexture()
{
    // free memory
    glDeleteTextures(1, &m_texture);
    glVertexPointer(2, GL_FLOAT, 0, m_vertices.constData());
    glTexCoordPointer(2, GL_FLOAT, 0, m_texCoords.constData());
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // Non-mipmap way of mapping the texture (fast and clean):		// Allocate the texture
    glGenTextures(1, &m_texture);
    // Select the texture.
    glBindTexture(GL_TEXTURE_2D, m_texture);
    // If texture area is larger then the image, upscale using no interpolation.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // If texture area is smaller than the image, downsample using no interpolation.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // create Texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_img.cols, m_img.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, m_img.data);
}

void TextureObject::setImage(const cv::Mat &img)
{
    assert(!img.empty());

    m_img = img;
    createVertices();
    createTexture();
}

}   // namespace Core
}   // namespace BioTracker

