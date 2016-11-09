#ifndef BIOTRACKER3TEXTUREOBJECT_H
#define BIOTRACKER3TEXTUREOBJECT_H

#include "Interfaces/imodel.h"

#include <opencv2/opencv.hpp>
#include "QImage"

class BioTracker3TextureObject : public IModel {
    Q_OBJECT
  public:
    explicit BioTracker3TextureObject(QObject *parent = 0);

    void set(cv::Mat const &img);

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
    cv::Mat m_img;
    QImage m_texture;
};

#endif // BIOTRACKER3TEXTUREOBJECT_H
