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

class TextureObject : public QObject {
  public:
    explicit TextureObject(QObject *parent);
    void draw() const;

    void setImage(const cv::Mat &img);
    const cv::Mat &getImage() const {
        return m_img;
    }

    QImage gen(const cv::Mat &img);

  private:
    QImage m_texture;
    cv::Mat m_img;
    cv::Mat m_imgTemp;
    std::mutex m_genMutex;

    void updateTexture(const cv::Mat &img);

    void createTexture();
    void createVertices();
};

}   // namespace Core
}   // namespace BioTracker


