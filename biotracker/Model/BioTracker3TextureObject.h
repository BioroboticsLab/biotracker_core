#ifndef BIOTRACKER3TEXTUREOBJECT_H
#define BIOTRACKER3TEXTUREOBJECT_H

#include "Interfaces/imodel.h"

#include <opencv2/opencv.hpp>
#include "QImage"
#include "QString"

class BioTracker3TextureObject : public IModel {
    Q_OBJECT
  public:
    explicit BioTracker3TextureObject(QObject *parent = 0, QString name = "NoName");

    void set(cv::Mat const &img);
    QString getName();

    QImage const &get() const {
        return m_texture;
    }
    int width() const {
        return m_texture.width();
    }
    int height() const {
        return m_texture.height();
    }

  private:
    QString m_Name;
    cv::Mat m_img;
    QImage m_texture;
};

#endif // BIOTRACKER3TEXTUREOBJECT_H
