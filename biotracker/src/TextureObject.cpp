#include "TextureObject.h"


QImage Mat2QImage(const cv::Mat &image2Draw_mat) {
    auto image2Draw_qt = QImage(
                             image2Draw_mat.data,
                             image2Draw_mat.cols,
                             image2Draw_mat.rows,
                             image2Draw_mat.step,
                             QImage::Format_RGB888);
    return image2Draw_qt;
}


namespace BioTracker {
namespace Core {

TextureObject::TextureObject(QObject *parent)
    : QObject(parent) {
    // OpenCV's coordinate system originates in the upper left corner.
    // OpenGL originates in the lower left. Thus the image has to be flipped vertically

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

    QImage pic = Mat2QImage(img);
}

QImage TextureObject::gen(const cv::Mat &img) {
    m_img = img;
    cv::cvtColor(img, img, cv::COLOR_BGR2RGB); // TODO copy into new  buffer
    QImage pic = Mat2QImage(img);
    return pic;

}

}   // namespace Core
}   // namespace BioTracker

