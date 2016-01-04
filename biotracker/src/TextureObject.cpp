#include "TextureObject.h"

namespace BioTracker {
namespace Core {

TextureObject::TextureObject(QObject *parent)
    : QObject(parent) {
    // OpenCV's coordinate system originates in the upper left corner.
    // OpenGL originates in the lower left. Thus the image has to be flipped vertically
    m_texture = QImage(1, 1, QImage::Format_RGB888);
    setImage(cv::Mat::zeros(1, 1, CV_8UC3));
}

void TextureObject::draw() const {
    //glEnable(GL_DEBUG_OUTPUT);
}

void TextureObject::createVertices() {
}

void TextureObject::createTexture() {
    // free memory
}

void TextureObject::setImage(const cv::Mat &img) {
    assert(!img.empty());
    m_img = img;
}

QImage TextureObject::gen(const cv::Mat &img) {
    m_genMutex.lock();
    m_img = img;
    updateTexture(img);
    m_genMutex.unlock();
    return m_texture;

}

void TextureObject::updateTexture(const cv::Mat &src) {
    cv::cvtColor(src, m_imgTemp, CV_BGR2RGB);
    m_texture = QImage(
                    m_imgTemp.data,
                    src.cols,
                    src.rows,
                    src.step,
                    QImage::Format_RGB888
                );
}

}   // namespace Core
}   // namespace BioTracker

