#include "ImageStream.h"

#include "util/stdext.h"
#include <cassert>    // assert
#include <stdexcept>  // std::invalid_argument

#include "settings/Exceptions.h"
#include "QSharedPointer"

namespace BioTracker {
namespace Core {

ImageStream::ImageStream(QObject *parent) : QObject(parent),
    m_current_frame(new cv::Mat(cv::Size(0,0), CV_8UC3)),
    m_current_frame_number(0) {

}

size_t ImageStream::currentFrameNumber() const {
    return m_current_frame_number;
}

std::shared_ptr<cv::Mat> ImageStream::currentFrame() const {
    return m_current_frame;
}

bool ImageStream::setFrameNumber(size_t frame_number) {
    // valid new frame number
    if (frame_number < this->numFrames()) {
        // skip update if frame number doesn't change
        if (frame_number == this->currentFrameNumber()) {
            return true;
        } else {
            const bool success = this->setFrameNumber_impl(frame_number);
            m_current_frame_number = frame_number;
            return success;
        }
    }
    // invalid new frame number
    else {
        this->clearImage();
        return false;
    }
}

bool ImageStream::lastFrame() const {
    return this->currentFrameNumber() + 1 == this->numFrames();
}

bool ImageStream::end() const {
    return this->currentFrameNumber() >= this->numFrames();
}

bool ImageStream::currentFrameIsEmpty() const {
    return this->currentFrame()->empty();
}

bool ImageStream::nextFrame() {
    const size_t new_frame_number = this->currentFrameNumber() + 1;
    if (new_frame_number < this->numFrames()) {
        const bool success = this->nextFrame_impl();
        m_current_frame_number = new_frame_number;
        return success;
    } else {
        this->clearImage();
        return false;
    }
}

bool ImageStream::previousFrame() {
    if (this->currentFrameNumber() > 0) {
        const size_t new_frame_numer = this->currentFrameNumber() - 1;
        const bool success = this->previousFrame_impl();
        m_current_frame_number = new_frame_numer;
        return success;
    } else {
        this->clearImage();
        return false;
    }
}

void ImageStream::set_current_frame(std::shared_ptr<cv::Mat> img) {
    m_current_frame.swap(img);

}

void ImageStream::clearImage() {
    m_current_frame.reset();
    m_current_frame_number = this->numFrames();
}

bool ImageStream::nextFrame_impl() {
    assert(this->currentFrameNumber() + 1 < this->numFrames());
    const size_t new_frame_number = this->currentFrameNumber() + 1;
    return this->setFrameNumber_impl(new_frame_number);
}

bool ImageStream::previousFrame_impl() {
    assert(this->currentFrameNumber() > 0);
    const size_t new_frame_number = this->currentFrameNumber() - 1;
    return this->setFrameNumber_impl(new_frame_number);
}

ImageStream::~ImageStream() = default;


/*********************************************************/


class ImageStream3NoMedia : public ImageStream {
  public:
    explicit ImageStream3NoMedia() = default;
    virtual GuiParam::MediaType type() const override {
        return GuiParam::MediaType::NoMedia;
    }
    virtual size_t numFrames() const override {
        return 0;
    }
    virtual double fps() const override {
        return 1.0;
    }
    virtual std::string currentFilename() const override {
        return "No Media"; // TODO make this nicer..
    }

  private:
    virtual bool setFrameNumber_impl(size_t) override {
        return false;
    }
};


/*********************************************************/


class ImageStream3Pictures : public ImageStream {
  public:
    explicit ImageStream3Pictures(std::vector<boost::filesystem::path> picture_files)
        : m_picture_files(std::move(picture_files)) {
        // load first image
        if (this->numFrames() > 0) {
            this->setFrameNumber_impl(0);
        }
    }
    virtual GuiParam::MediaType type() const override {
        return GuiParam::MediaType::Images;
    }
    virtual size_t numFrames() const override {
        return m_picture_files.size();
    }
    virtual double fps() const override {
        return 1.0;
    }
    virtual std::string currentFilename() const override {
        assert(currentFrameNumber() < m_picture_files.size());
        return m_picture_files[currentFrameNumber()].string();
    }

  private:
    virtual bool setFrameNumber_impl(size_t frame_number) override {
        const std::string &filename = m_picture_files[frame_number].string();
        std::shared_ptr<cv::Mat> new_frame (new cv::Mat(cv::imread(filename)));
        this->set_current_frame(new_frame);
        return ! new_frame->empty();
    }
    std::vector<boost::filesystem::path> m_picture_files;
};


/*********************************************************/


class ImageStream3Video : public ImageStream {
  public:
    /**
     * @throw file_not_found when the file does not exists
     * @throw video_open_error when there is an error with the video
     * @brief ImageStreamVideo
     * @param filename path to the file
     */
    explicit ImageStream3Video(const boost::filesystem::path &filename)
        : m_capture(filename.string())
        , m_num_frames(static_cast<size_t>(m_capture.get(CV_CAP_PROP_FRAME_COUNT)))
        , m_fps(m_capture.get(CV_CAP_PROP_FPS))
        , m_fileName(filename.string()) {
        if (!boost::filesystem::exists(filename)) {
            throw file_not_found("Could not find file " + filename.string());
        }
        if (! m_capture.isOpened()) {
            throw video_open_error(":(");
        }
        // load first image
        if (this->numFrames() > 0) {
            this->nextFrame_impl();
        }
    }
    virtual GuiParam::MediaType type() const override {
        return GuiParam::MediaType::Video;
    }
    virtual size_t numFrames() const override {
        return m_num_frames;
    }
    virtual double fps() const override {
        return m_fps;
    }
    virtual std::string currentFilename() const override {
        return m_fileName;
    }

  private:
    virtual bool nextFrame_impl() override {
        cv::Mat new_frame;
        m_capture >> new_frame;
        std::shared_ptr<cv::Mat> mat(new cv::Mat(new_frame));
        this->set_current_frame(mat);
        return ! new_frame.empty();
    }

    virtual bool setFrameNumber_impl(size_t frame_number) override {
        // new frame is next frame --> use next frame function
        if (this->currentFrameNumber() + 1 == frame_number) {
            return this->nextFrame_impl();
        } else {
            // adjust frame position ("0-based index of the frame to be decoded/captured next.")
            m_capture.set(CV_CAP_PROP_POS_FRAMES, static_cast<double>(frame_number));
            return this->nextFrame_impl();
        }
    }

    cv::VideoCapture m_capture;
    const size_t     m_num_frames;
    const double     m_fps;
    const std::string m_fileName;
};


/*********************************************************/

class ImageStream3Camera : public ImageStream {
  public:
    /**
     * @throw file_not_found when device does not exists
     * @throw device_open_error when there is an error with the device
     * @brief ImageStreamCamera
     * @param device_id according to the VideoCapture class of OpenCV
     */
    explicit ImageStream3Camera(int device_id)
        : m_capture(device_id)
        , m_fps(m_capture.get(CV_CAP_PROP_FPS)) {
        if (! m_capture.isOpened()) {
            throw device_open_error(":(");
        }
        // load first image
        if (this->numFrames() > 0) {
            this->nextFrame_impl();
        }
    }
    virtual GuiParam::MediaType type() const override {
        return GuiParam::MediaType::Camera;
    }
    virtual size_t numFrames() const override {
        return 10000;
    }
    virtual double fps() const override {
        return m_fps;
    }
    virtual std::string currentFilename() const override {
        return "Camera"; // TODO be more specific!
    }

  private:
    virtual bool nextFrame_impl() override {
        cv::Mat new_frame;
        m_capture.grab();
        m_capture.retrieve(new_frame);
        std::shared_ptr<cv::Mat> mat (new cv::Mat(new_frame));
        this->set_current_frame(mat);
        return ! mat->empty();
    }

    virtual bool setFrameNumber_impl(size_t) override {
//        throw std::runtime_error("setFrameNumber not available for ImageStreamCamera");
        this->nextFrame_impl();
        return true;
    }

    cv::VideoCapture m_capture;
    const double m_fps;
};

/*********************************************************/


std::shared_ptr<ImageStream> make_ImageStream3NoMedia() {
    return std::make_shared<ImageStream3NoMedia>();
}

std::shared_ptr<ImageStream> make_ImageStream3Pictures(
    std::vector<boost::filesystem::path> filenames) {
    return std::make_shared<ImageStream3Pictures>(std::move(filenames));
}

std::shared_ptr<ImageStream> make_ImageStream3Video(const boost::filesystem::path
        &filename) {
    try {
        return std::make_shared<ImageStream3Video>(filename);
    } catch (const video_open_error &) {
        return make_ImageStream3NoMedia();
    }
}

std::shared_ptr<ImageStream> make_ImageStream3Camera(int device) {
    try {
        return std::make_shared<ImageStream3Camera>(device);
    } catch (const device_open_error &) {
        return make_ImageStream3NoMedia();
    }
}

}
}
