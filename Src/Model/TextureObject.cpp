#include "TextureObject.h"

TextureObject::TextureObject(QObject *parent, QString name) :
    IModel(parent),
    m_Name(name)
{
    // OpenCV's coordinate system originates in the upper left corner.
    // OpenGL originates in the lower left. Thus the image has to be flipped vertically
    m_texture = QImage(1, 1, QImage::Format_RGB888);
}

void TextureObject::set(std::shared_ptr<cv::Mat> img) {
    if (img->empty()) {
        return;
    }

    if (img->channels() == 3) {
        img->convertTo(m_img, CV_8UC3);
        cv::cvtColor(m_img, m_img, cv::ColorConversionCodes::COLOR_BGR2RGB);
    } else if (img->channels() == 1) {
        // convert grayscale to "color"
        cv::Mat img8U;

        // we assume that the 1d image has more than 8bit per pixel
        // (usually 64F) so we need to map a [HUGE range] to -> [0 .. 255]
        double min, max;
        // FIXME: This still sometimes crashes when accessing the img data.
        //        cv::Mat is reference counted, so all std::shared_ptr<cv::Mat>
        //        instances should be replaced with cv::Mat.
        cv::minMaxLoc(*img, &min, &max);
        if (min >= 0 && min < 255 && max >= 0 && max <= 255) {
            // do not refit if the range is actually inbetween [0 ... 255]
            img->convertTo(img8U, CV_8U);
        } else if (max > min) {
            // otherwise: the range is outside of native [0 ... 255] so we
            // actually need to do some refitting

            // mapping 1-step out from [0 .. 255] range 1-step in the [min .. max] range
            const double sizeRatio = 256.0/abs(static_cast<int>(max - min));
            const double convertedMin = abs(static_cast<int>(min * sizeRatio));
            img->convertTo(img8U, CV_8U, sizeRatio, convertedMin);
        }
        cv::cvtColor(img8U, m_img, cv::ColorConversionCodes::COLOR_GRAY2RGB);
    } else {
        img->copyTo(m_img);
    }

    m_texture = QImage(
                    m_img.data,
                    m_img.cols,
                    m_img.rows,
                    static_cast<int>(m_img.step),
                    QImage::Format_RGB888
                );

    Q_EMIT notifyView();
}

QString TextureObject::getName()
{
    return m_Name;
}
