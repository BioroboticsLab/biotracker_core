#pragma once

#include <opencv2/opencv.hpp>

#include <QObject>
#include <QtOpenGL>
#include <atomic>
#include <memory>
#include <condition_variable>
#include <QThread>

namespace BioTracker {
namespace Core {

class TextureObject {
  public:
    explicit TextureObject();

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

}   // namespace Core
}   // namespace BioTracker


