#pragma once

#include <memory>                       // std::unique_ptr
#include <opencv2/opencv.hpp>           // cv::Mat
#include <vector>                       // std::vector
#include <string>                       // std::string
#include <boost/filesystem.hpp>

#include "biotracker/core/settings/ParamNames.h" // GUIPARAM::MediaType

namespace BioTracker {
namespace Core {

class ImageStream {
  public:
    explicit ImageStream();

    /**
     * @return the stream's MediaType i.e. "Video", "Images", "NoMedia"
     */
    virtual GUIPARAM::MediaType type() const = 0;

    /**
     * @return the total number of frames
     */
    virtual size_t numFrames() const = 0;

    /**
     * @return the current frame number
     */
    size_t currentFrameNumber() const;

    /**
     * @return true, if the current frame is the last frame
     */
    bool lastFrame() const;

    /**
     * @return true, if the stream's end is reached; i.e. current frame points behind it's last frame
     */
    bool end() const;

    /**
     * @return true, if currentFrame().empty();
     */
    bool currentFrameIsEmpty() const;

    /**
     * @return the media's fps rate
     */
    virtual double fps() const = 0;

    /**
     * returns the current frame.
     * - if the current frame position is invalid or an error occurred, an empty image is returned.
     */
    const cv::Mat &currentFrame() const;

    /**
     * sets the current frame number and updates the current frame.
     * - if frame_number is invalid, the current frame is invalidated.
     * @return true if the operation was successful, i.e. the frame number is valid and no error occurred.
     */
    bool setFrameNumber(size_t frame_number);

    /**
     * advances the current frame frame.
     * - if this function is called on the media's last frame, the current frame is invalidated.
     * @return true if the operation was successful, i.e. the current frame isn't the last frame and no error occurred.
     */
    bool nextFrame();

    /**
     * sets the current frame position to the previous frame.
     * - if this function is called on the media's first frame, the current frame is invalidated.
     * @return true if the operation was successful, i.e. the current frame isn't the first one and no error occurred.
     */
    bool previousFrame();

    virtual ~ImageStream();

  protected:
    /**
     * sets the image returned by this->currentFrame();
     */
    void set_current_frame(const cv::Mat &img);

  private:
    /**
     * empties m_current_frame & sets m_current_frame_number to this->numFrames();
     */
    void clearImage();
    /**
     * - called by ImageStreamImpl::setFrameNumber
     *    if frame_number < numFrames() && frame_number != this->currentFrameNumber();
     * - m_current_frame_number is updated afterwards
     */
    virtual bool setFrameNumber_impl(size_t frame_number) = 0;
    /**
     * - called by ImageStreamImpl::nextFrame()
     *    if currentFrameNumber() + 1 < numFrames();
     * - m_current_frame_number is updated afterwards
     */
    virtual bool nextFrame_impl();
    /**
     * - called by ImageStreamImpl::previousFrame()
     *    if currentFrameNumber() > 0;
     * - m_current_frame_number is updated afterwards
     */
    virtual bool previousFrame_impl();

    cv::Mat m_current_frame;
    size_t  m_current_frame_number;
};

std::unique_ptr<ImageStream> make_ImageStreamNoMedia();

std::unique_ptr<ImageStream> make_ImageStreamPictures(
    std::vector<boost::filesystem::path> filenames);

std::unique_ptr<ImageStream> make_ImageStreamVideo(const boost::filesystem::path
        &filename);

std::unique_ptr<ImageStream> make_ImageStreamCamera(int device);

}
}
