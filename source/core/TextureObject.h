#pragma once

#include <opencv2/opencv.hpp>

#include <QObject>
#include <QtOpenGL>

namespace BioTracker{
namespace Core {

class TextureObject : public QObject
{
public:
    explicit TextureObject(QObject *parent, QOpenGLContext *context);
    void draw() const;

    void setImage(const cv::Mat &img);
    const cv::Mat &getImage() const { return m_img; }

private:
    cv::Mat m_img;
    GLuint m_texture;
    QOpenGLContext *m_context;

    QVector<QVector2D> m_vertices;
    QVector<QVector2D> m_texCoords;

    void createTexture();
    void createVertices();
};

}   // namespace Core
}   // namespace BioTracker


