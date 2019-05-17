/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** Origin of this class is BioTracker in version 2.
  **
  ****************************************************************************/

#ifndef BIOTRACKER3IMAGESTREAM_H
#define BIOTRACKER3IMAGESTREAM_H

#include <memory>                       // std::unique_ptr
#include <opencv2/opencv.hpp>           // cv::Mat
#include <vector>                       // std::vector
#include <string>                       // std::string
#include <boost/filesystem.hpp>
#include <QObject>
#include "QSharedPointer"

#include "util/ParamNames.h" // GUIPARAM::MediaType
#include "util/types.h"
#include "util/camera/base.h"
#include "util/Config.h"

namespace BioTracker {
namespace Core {

/**
 * The ImageStream class was part of BioTracker version 2. It is responsible for generation ImageStreams from files or camera devices.
 */
class ImageStream : public QObject {
    Q_OBJECT
  public:
    explicit ImageStream(QObject* parent = 0, Config *cfg = nullptr);

    /**
         * @return the stream's MediaType i.e. "Video", "Images", "NoMedia"
         */
    virtual GuiParam::MediaType type() const = 0;

    /**
     * @return the total number of frames
     */
    virtual size_t numFrames() const = 0;

	virtual bool toggleRecord() = 0;

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
     * @return the filename of the current file defined through the frameNumber
     */
    virtual std::string currentFilename() const = 0;

    /**
     * returns the current frame.
     * - if the current frame position is invalid or an error occurred, an empty image is returned.
     */
    std::shared_ptr<cv::Mat> currentFrame() const;

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

	/**
	* Gets the title of the current image stream.
	* A title should represent the identity of a source stream as a string.
	* E.g. a video "MyTracking.mkv" should yield a name like "MyTracking".
	*		Streams from Camera No. 1 should yiel "Camera_1" or the like.
	* Naming conventions are up to the stream implementations.
	*/
	std::string getTitle();

    virtual bool hasNextInBatch();

	virtual void stepToNextInBatch();

    virtual std::vector<std::string> getBatchItems();

    virtual ~ImageStream();

  protected:
    /**
     * sets the image returned by this->currentFrame();
     */
    void set_current_frame(std::shared_ptr<cv::Mat> img);

	/**
	* Sets the title of the current image stream.
	* A title should represent the identity of a source stream as a string.
	* E.g. a video "MyTracking.mkv" should yield a name like "MyTracking".
	*		Streams from Camera No. 1 should yiel "Camera_1" or the like.
	* Naming conventions are up to the stream implementations. 
	* The title is set by the stream itself and may not be modified from the outside.
	*/
    void setTitle(std::string title);

	/**
	* The stride of the image stream. Think of it as "use only every n'th frame".
	*/
	size_t m_frame_stride;

    std::shared_ptr<cv::Mat> m_current_frame;
	size_t  m_current_frame_number;
	std::string m_title;
    Config *_cfg;

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
};

std::shared_ptr<ImageStream> make_ImageStream3NoMedia();

std::shared_ptr<ImageStream> make_ImageStream3Pictures(Config *cfg,
    std::vector<boost::filesystem::path> filenames);

std::shared_ptr<ImageStream> make_ImageStream3Video(Config *cfg, const std::vector<boost::filesystem::path>
                                                    &filename);

std::shared_ptr<ImageStream> make_ImageStream3Camera(Config *cfg, CameraConfiguration conf);

}
}

#endif // BIOTRACKER3IMAGESTREAM_H
