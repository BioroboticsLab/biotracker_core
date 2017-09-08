#include "ImageStream.h"

#include "util/stdext.h"
#include <cassert>    // assert
#include <stdexcept>  // std::invalid_argument
#include <chrono>
#include <thread>

#include "util/Exceptions.h"
#include "QSharedPointer"
#include "settings/Settings.h"
#include "util/misc.h"

#include "View/CameraDevice.h"
#include "util/types.h"
#include "util/singleton.h"
#include "settings/Settings.h"
#include "util/VideoCoder.h"

namespace BioTracker {
	namespace Core {

		ImageStream::ImageStream(QObject *parent) : QObject(parent),
			m_current_frame(new cv::Mat(cv::Size(0, 0), CV_8UC3)),
			m_current_frame_number(0),
			m_frame_stride(BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION)->
				getValueOrDefault<int>(CFG_INPUT_FRAME_STRIDE, CFG_INPUT_FRAME_STRIDE_VAL)) {
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
			return (this->currentFrameNumber() >= this->numFrames() && this->numFrames() >= 0);
		}

		bool ImageStream::currentFrameIsEmpty() const {
			return this->currentFrame()->empty();
		}

		bool ImageStream::nextFrame() {
			const size_t new_frame_number = this->currentFrameNumber() + m_frame_stride;
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

		bool ImageStream::previousFrame() {
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
			virtual bool toggleRecord() override {
				return false;
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
				: m_picture_files(std::move(picture_files)), m_currentFrame(0) {

				//Grab the codec from config file
				BioTracker::Core::Settings *set = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
				m_fps = set->getValueOrDefault<double>(CFG_RECORD_FPS, CFG_RECORD_FPS_VAL);

				// load first image
				if (this->numFrames() > 0) {
					this->setFrameNumber_impl(0);
					std::string filename = m_picture_files[0].string();
					std::shared_ptr<cv::Mat> new_frame = std::make_shared<cv::Mat>(cv::imread(filename));
					m_w = new_frame->size().width;
					m_h = new_frame->size().height;
					m_recording = false;
					vCoder = std::make_shared<VideoCoder>();
				}

			}
			virtual GuiParam::MediaType type() const override {
				return GuiParam::MediaType::Images;
			}
			virtual size_t numFrames() const override {
				return m_picture_files.size();
			}
			virtual bool toggleRecord() override {
				if (this->numFrames() <= 0) {
					return false;
				}
				m_recording = vCoder->toggle(1, m_w, m_h);

				return m_recording;
			}
			virtual double fps() const override {
				return m_fps;
			}
			virtual std::string currentFilename() const override {
				assert(currentFrameNumber() < m_picture_files.size());
				return m_picture_files[currentFrameNumber()].string();
			}

		private:
			virtual bool nextFrame_impl() override {
				m_currentFrame += m_frame_stride;
				if (this->numFrames() > m_currentFrame) {

					const std::string &filename = m_picture_files[m_currentFrame].string();
					std::shared_ptr<cv::Mat> new_frame = std::make_shared<cv::Mat>(cv::imread(filename));
					this->set_current_frame(new_frame);
					if (m_recording) {
						if (vCoder) vCoder->add(new_frame);
					}
					return true;
				}
				return false;
			}

			virtual bool setFrameNumber_impl(size_t frame_number) override {
				const std::string &filename = m_picture_files[frame_number].string();
				std::shared_ptr<cv::Mat> new_frame = std::make_shared<cv::Mat>(cv::imread(filename));
				this->set_current_frame(new_frame);
				m_currentFrame = frame_number;
				if (m_recording) {
					if (vCoder) vCoder->add(new_frame);
				}
				return !new_frame->empty();
			}
			std::vector<boost::filesystem::path> m_picture_files;
			std::shared_ptr<VideoCoder> vCoder;
			double m_w;
			double m_h;
			bool m_recording;
			int m_currentFrame;
			double m_fps;
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
				if (!m_capture.isOpened()) {
					throw video_open_error(":(");
				}
				m_w = m_capture.get(CV_CAP_PROP_FRAME_WIDTH);
				m_h = m_capture.get(CV_CAP_PROP_FRAME_HEIGHT);
				m_recording = false;
				vCoder = std::make_shared<VideoCoder>();

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
			virtual bool toggleRecord() override {
				if (!m_capture.isOpened()) {
					return false;
				}
				m_recording = vCoder->toggle(m_fps, m_w, m_h);

				return m_recording;
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
				for (int i = 0; i<m_frame_stride; i++)
					m_capture >> new_frame;
				std::shared_ptr<cv::Mat> mat(new cv::Mat(new_frame));
				this->set_current_frame(mat);
				if (m_recording) {
					if (vCoder) vCoder->add(mat);
				}
				return !new_frame.empty();
			}

			virtual bool setFrameNumber_impl(size_t frame_number) override {
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
			const std::string m_fileName;
			std::shared_ptr<VideoCoder> vCoder;
			double m_fps;
			double m_w;
			double m_h;
			bool m_recording;
		};


		/*********************************************************/
#include <chrono>
		class ImageStream3Camera : public ImageStream {
		public:
			/**
			* @throw file_not_found when device does not exists
			* @throw device_open_error when there is an error with the device
			* @brief ImageStreamCamera
			* @param device_id according to the VideoCapture class of OpenCV
			*/
			explicit ImageStream3Camera(CameraConfiguration conf)
				: m_capture(conf._id)
				, m_fps(m_capture.get(CV_CAP_PROP_FPS)) {
				// Give the camera some extra time to get ready:
				// Somehow opening it on first try sometimes does not succeed.
				// Workaround: http://stackoverflow.com/questions/22019064/unable-to-read-frames-from-videocapture-from-secondary-webcam-with-opencv?rq=1
				// So, stubbornly try it a few times until it works.
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));

				Settings *set = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);

				std::cout << "\nStarting to record on camera no. " << conf._id << std::endl;
				m_w = conf._width == -1 ? set->getValueOrDefault<int>(CFG_CAMERA_DEFAULT_W, CFG_CAMERA_DEFAULT_W_VAL) : conf._width;
				m_h = conf._height == -1 ? set->getValueOrDefault<int>(CFG_CAMERA_DEFAULT_H, CFG_CAMERA_DEFAULT_H_VAL) : conf._height;
				m_fps = conf._fps == -1 ? set->getValueOrDefault<int>(CFG_RECORD_FPS, CFG_RECORD_FPS_VAL) : conf._fps;
				m_recording = false;
				vCoder = std::make_shared<VideoCoder>();

				int fails = 0;
				while (!m_capture.isOpened() && fails < 5) {
					m_capture.open(conf._id);
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					fails++;
				}

				if (!m_capture.isOpened()) {
					std::cout << "Unable to open camera!" << std::endl;
					throw device_open_error(":(");
				}

				if (m_w != -1)     m_capture.set(CV_CAP_PROP_FRAME_WIDTH, m_w);
				if (m_h != -1)     m_capture.set(CV_CAP_PROP_FRAME_HEIGHT, m_h);
				if (m_fps != -1) m_capture.set(CV_CAP_PROP_FPS, m_fps);

				m_w = m_capture.get(CV_CAP_PROP_FRAME_WIDTH);
				m_h = m_capture.get(CV_CAP_PROP_FRAME_HEIGHT);
				m_fps = m_capture.get(CV_CAP_PROP_FPS);
				std::cout << "Cam open: " << m_capture.isOpened() << " w/h:" << m_w << "/" << m_h << " fps:" << m_fps << std::endl;
				// load first image
				if (this->numFrames() > 0) {
					this->nextFrame_impl();
				}
			}
			virtual GuiParam::MediaType type() const override {
				return GuiParam::MediaType::Camera;
			}
			virtual size_t numFrames() const override {
				return -1; //TODO
			}
			virtual bool toggleRecord() override {
				if (!m_capture.isOpened()) {
					return false;
				}
				m_recording = vCoder->toggle(m_fps, m_w, m_h);

				return m_recording;
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

				for (int i = 0; i < m_frame_stride; i++) {
					m_capture >> new_frame;
				}

				std::shared_ptr<cv::Mat> mat(new cv::Mat(new_frame));
				this->set_current_frame(mat);
				if (m_recording) {
					if (vCoder) vCoder->add(mat);
				}
				return !mat->empty();
			}

			virtual bool setFrameNumber_impl(size_t) override {
				//        throw std::runtime_error("setFrameNumber not available for ImageStreamCamera");
				this->nextFrame_impl();
				return true;
			}

			std::shared_ptr<VideoCoder> vCoder;
			cv::VideoCapture m_capture;
			double m_fps;
			double m_w;
			double m_h;
			bool m_recording;
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
			}
			catch (const video_open_error &) {
				return make_ImageStream3NoMedia();
			}
		}

		std::shared_ptr<ImageStream> make_ImageStream3Camera(CameraConfiguration conf) {
			try {
				return std::make_shared<ImageStream3Camera>(conf);
			}
			catch (const device_open_error &) {
				return make_ImageStream3NoMedia();
			}
		}

	}
}
