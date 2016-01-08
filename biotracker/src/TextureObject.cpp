#include "TextureObject.h"

namespace BioTracker {
namespace Core {

TextureObject::TextureObject() {
    // OpenCV's coordinate system originates in the upper left corner.
    // OpenGL originates in the lower left. Thus the image has to be flipped vertically
    m_texture = QImage(1, 1, QImage::Format_RGB888);
}

void TextureObject::set(cv::Mat const &img) {
    cv::cvtColor(img, m_img, CV_BGR2RGB);
    m_texture = QImage(
                    m_img.data,
                    m_img.cols,
                    m_img.rows,
                    m_img.step,
                    QImage::Format_RGB888
                );
}


}   // namespace Core
}   // namespace BioTracker

