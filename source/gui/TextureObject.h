#ifndef TEXTUREOBJECT_H
#define TEXTUREOBJECT_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <QtOpenGL>

class TextureObject : public QObject
{
public:
    explicit TextureObject(QObject *parent);
    ~TextureObject();
    void setPicture(cv::Mat picture);
    void draw() const;
    void init();
private:
    GLuint _texture;
    int _cols, _rows;
    QVector<QVector2D> _vertices;
    QVector<QVector2D> _texCoords;
    void createTexture(cv::Mat picture);
    void createVertices(int cols, int rows);
};

#endif // TEXTUREOBJECT_H
