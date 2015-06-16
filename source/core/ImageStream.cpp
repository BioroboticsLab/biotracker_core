/*
 * ImageStream.cpp
 *
 *  Created on: May 13, 2015
 *      Author: tobias
 */

#include "ImageStream.h"
#include <cassert>    // assert
#include <stdexcept>  // std::invalid_argument


/*********************************************************/

struct video_open_error : std::invalid_argument {
	using std::invalid_argument::invalid_argument;
};

/*********************************************************/


ImageStream::ImageStream()
	: m_current_frame(cv::Size(0,0), CV_8UC3)
	, m_current_frame_number(0)
{}

size_t ImageStream::currentFrameNumber() const {
	return m_current_frame_number;
}

const cv::Mat& ImageStream::currentFrame() const {
	return m_current_frame;
}

bool ImageStream::setFrameNumber(size_t frame_number)
{
	// valid new frame number
	if (frame_number < this->numFrames())
	{
		// skip update if frame number doesn't change
		if (frame_number == this->currentFrameNumber()) {
			return true;
		}
		else {
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
	return this->currentFrame().empty();
}

bool ImageStream::nextFrame()
{
	const size_t new_frame_number = this->currentFrameNumber() + 1;
	if (new_frame_number < this->numFrames()) {
		const bool success = this->nextFrame_impl();
		m_current_frame_number = new_frame_number;
		return success;
	}
	else {
		this->clearImage();
		return false;
	}
}

bool ImageStream::previousFrame()
{
	if (this->currentFrameNumber() > 0) {
		const size_t new_frame_numer = this->currentFrameNumber() - 1;
		const bool success = this->previousFrame_impl();
		m_current_frame_number = new_frame_numer;
		return success;
	}
	else {
		this->clearImage();
		return false;
	}
}

void ImageStream::set_current_frame(const cv::Mat &img) {
	m_current_frame = img;
}

void ImageStream::clearImage() {
	m_current_frame.release();
	m_current_frame_number = this->numFrames();
}

bool ImageStream::nextFrame_impl()
{
	assert( this->currentFrameNumber() + 1 < this->numFrames() );
	const size_t new_frame_number = this->currentFrameNumber() + 1;
	return this->setFrameNumber_impl(new_frame_number);
}

bool ImageStream::previousFrame_impl()
{
	assert( this->currentFrameNumber() > 0 );
	const size_t new_frame_number = this->currentFrameNumber() - 1;
	return this->setFrameNumber_impl(new_frame_number);
}

ImageStream::~ImageStream() = default;


/*********************************************************/


class ImageStreamNoMedia : public ImageStream
{
public:
	explicit ImageStreamNoMedia() = default;
	virtual GUIPARAM::MediaType type() const override { return GUIPARAM::MediaType::NoMedia; }
	virtual size_t numFrames() const override { return 0; }
	virtual double fps() const override { return 1.0; }
private:
	virtual bool setFrameNumber_impl(size_t) override { return false; }
};


/*********************************************************/


class ImageStreamPictures : public ImageStream
{
public:
    explicit ImageStreamPictures(std::vector<boost::filesystem::path> picture_files)
		: m_picture_files(std::move(picture_files))
	{
		// load first image
		if (this->numFrames() > 0) {
			this->setFrameNumber_impl(0);
		}
	}
	virtual GUIPARAM::MediaType type() const override { return GUIPARAM::MediaType::Images; }
	virtual size_t numFrames() const override { return m_picture_files.size(); }
	virtual double fps() const override { return 1.0; }
private:
	virtual bool setFrameNumber_impl(size_t frame_number) override
	{
        const std::string& filename = m_picture_files[frame_number].string();
		const cv::Mat new_frame = cv::imread(filename);
		this->set_current_frame( new_frame );
		return ! new_frame.empty();
	}
    std::vector<boost::filesystem::path> m_picture_files;
};


/*********************************************************/


class ImageStreamVideo : public ImageStream
{
public:
    explicit ImageStreamVideo(const boost::filesystem::path &filename)
        : m_capture(filename.string())
		, m_num_frames( static_cast<size_t>(m_capture.get(CV_CAP_PROP_FRAME_COUNT)) )
		, m_fps( m_capture.get(CV_CAP_PROP_FPS) )
	{
		if (! m_capture.isOpened()) {
			throw video_open_error(":(");
		}
		// load first image
		if (this->numFrames() > 0) {
			this->nextFrame_impl();
		}
	}
	virtual GUIPARAM::MediaType type() const override { return GUIPARAM::MediaType::Video; }
	virtual size_t numFrames() const override { return m_num_frames; }
	virtual double fps() const override { return m_fps; }
private:
	virtual bool nextFrame_impl() override
		{
			cv::Mat new_frame;
			m_capture >> new_frame;
			this->set_current_frame(new_frame);
			return ! new_frame.empty();
		}

		virtual bool setFrameNumber_impl(size_t frame_number) override
		{
			// new frame is next frame --> use next frame function
			if (this->currentFrameNumber() + 1 == frame_number) {
				return this->nextFrame_impl();
			}
			else {
				// adjust frame position ("0-based index of the frame to be decoded/captured next.")
				m_capture.set(CV_CAP_PROP_POS_FRAMES, static_cast<double>(frame_number));
				return this->nextFrame_impl();
			}
		}

	cv::VideoCapture m_capture;
	const size_t     m_num_frames;
	const double     m_fps;
};


/*********************************************************/


std::unique_ptr<ImageStream> make_ImageStreamNoMedia() {
	return std::make_unique<ImageStreamNoMedia>();
}

std::unique_ptr<ImageStream> make_ImageStreamPictures(std::vector<boost::filesystem::path> filenames) {
	return std::make_unique<ImageStreamPictures>(std::move(filenames));
}

std::unique_ptr<ImageStream> make_ImageStreamVideo(const boost::filesystem::path &filename) {
	try {
        return std::make_unique<ImageStreamVideo>(filename);
	}
	catch (const video_open_error &) {
		return make_ImageStreamNoMedia();
	}
}
