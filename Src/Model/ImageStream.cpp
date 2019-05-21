#include "ImageStream.h"

#include "util/stdext.h"
#include <cassert>    // assert
#include <stdexcept>  // std::invalid_argument
#include <chrono>
#include <thread>
#include <limits>

#include "util/Exceptions.h"
#include "QSharedPointer"
#include "Utility/misc.h"
#include "View/CameraDevice.h"
#include "util/VideoCoder.h"

#include "Controller/IControllerCfg.h"

#if HAS_PYLON
#include "util/camera/pylon.h"
#include <opencv2/opencv.hpp>
#endif
#include <iostream>

namespace BioTracker {
	namespace Core {

		ImageStream::ImageStream(QObject *parent, Config *cfg) : QObject(parent),
			m_current_frame(new cv::Mat(cv::Size(0, 0), CV_8UC3)),
			m_current_frame_number(0) {
			_cfg = cfg;
			if (cfg)
				m_frame_stride = cfg->FrameStride;
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

		bool ImageStream::hasNextInBatch(){
			return false;
		}

		void ImageStream::stepToNextInBatch(){
		}

		std::vector<std::string> ImageStream::getBatchItems(){
			return {};
		}

		ImageStream::~ImageStream() = default;


		/*********************************************************/


		class ImageStream3NoMedia : public ImageStream {
		public:
			explicit ImageStream3NoMedia() = default ;
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
			explicit ImageStream3Pictures(Config *cfg, std::vector<boost::filesystem::path> picture_files)
				:  ImageStream(0, cfg)
				,m_picture_files(std::move(picture_files)), m_currentFrame(0) {

				//Grab the codec from config file
                double fps = _cfg->RecordFPS;
                if (fps > 0) {
                    m_fps = fps;
                }
                else
                {
                    m_fps = 1;
                }

				// load first image
				if (this->numFrames() > 0) {
					this->setFrameNumber_impl(0);
					std::string filename = m_picture_files[0].string();
					std::shared_ptr<cv::Mat> new_frame = std::make_shared<cv::Mat>(cv::imread(filename));
					m_w = new_frame->size().width;
					m_h = new_frame->size().height;
					m_recording = false;
					vCoder = std::make_shared<VideoCoder>(m_fps, _cfg);
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
				m_recording = vCoder->toggle(m_w, m_h, m_fps);

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
			explicit ImageStream3Video(Config *cfg, const std::vector<boost::filesystem::path> &files) 
				: ImageStream(0, cfg)
			{
				openMedia(files);
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
				m_recording = vCoder->toggle(m_w, m_h, m_fps);

				return m_recording;
			}
			virtual double fps() const override {
				return m_fps;
			}
			virtual std::string currentFilename() const override {
				return m_fileName;
			}
			
			virtual bool hasNextInBatch() override{
				return !(m_batch.empty());
			}

			virtual void stepToNextInBatch() override{
				if (m_batch.empty()) {
					throw video_open_error("batch is empty");
				}
				openMedia(m_batch);
			} 

			virtual std::vector<std::string> getBatchItems() override{
				std::vector<std::string> batchItems;
				for (auto x: m_batch){
					batchItems.push_back(x.string());
				}
				return batchItems;
			}

		private:

			void openMedia(std::vector<boost::filesystem::path> files){

				m_capture.open(files.front().string());
				m_num_frames = static_cast<size_t>(m_capture.get(CV_CAP_PROP_FRAME_COUNT));
				m_fps = m_capture.get(CV_CAP_PROP_FPS);
				m_fileName = files.front().string();

				if (!boost::filesystem::exists(files.front())) {
					throw file_not_found("Could not find file " + files.front().string());
				}
				if (!m_capture.isOpened()) {
					throw video_open_error(":(");
				}

                m_batch = files;
                m_batch.erase(m_batch.begin(), m_batch.begin()+1);

                //Grab the fps from config file
                double fps = _cfg->RecordFPS;
                if (fps != -1) {
                    m_fps = fps;
                }

				m_w = m_capture.get(CV_CAP_PROP_FRAME_WIDTH);
				m_h = m_capture.get(CV_CAP_PROP_FRAME_HEIGHT);
				m_recording = false;
				vCoder = std::make_shared<VideoCoder>(m_fps, _cfg);

				// load first image
				if (this->numFrames() > 0) {
					this->nextFrame_impl();
				}

				m_current_frame_number = 0;
			}

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
			size_t     m_num_frames;
			std::string m_fileName;
			std::shared_ptr<VideoCoder> vCoder;
            std::vector<boost::filesystem::path> m_batch;
			double m_fps;
			double m_w;
			double m_h;
			bool m_recording;
		};


		/*********************************************************/
		class ImageStream3OpenCVCamera : public ImageStream {
		public:
			/**
			* @throw file_not_found when device does not exists
			* @throw device_open_error when there is an error with the device
			* @brief ImageStreamCamera
			* @param device_id according to the VideoCapture class of OpenCV
			*/
			explicit ImageStream3OpenCVCamera(Config *cfg, CameraConfiguration conf)
				: ImageStream(0, cfg)
				, m_capture(conf._selector.index)
				, m_fps(m_capture.get(CV_CAP_PROP_FPS)) {
				// Give the camera some extra time to get ready:
				// Somehow opening it on first try sometimes does not succeed.
				// Workaround: http://stackoverflow.com/questions/22019064/unable-to-read-frames-from-videocapture-from-secondary-webcam-with-opencv?rq=1
				// So, stubbornly try it a few times until it works.
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));

				qDebug() << "\nStarting to record on camera no. " << conf._selector.index;
				m_w = conf._width == -1 ? _cfg->CameraWidth : conf._width;
				m_h = conf._height == -1 ? _cfg->CameraHeight : conf._height;
				m_fps = conf._fps == -1 ? _cfg->RecordFPS : conf._fps;
				m_recording = false;
				vCoder = std::make_shared<VideoCoder>(m_fps, _cfg);

				int fails = 0;
				while (!m_capture.isOpened() && fails < 5) {
					m_capture.open(conf._selector.index);
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					fails++;
				}

				if (!m_capture.isOpened()) {
					qWarning() << "Unable to open camera!";
					throw device_open_error(":(");
				}

				if (m_w != -1)     m_capture.set(CV_CAP_PROP_FRAME_WIDTH, m_w);
				if (m_h != -1)     m_capture.set(CV_CAP_PROP_FRAME_HEIGHT, m_h);
				if (m_fps != -1)   m_capture.set(CV_CAP_PROP_FPS, m_fps);

				m_w = m_capture.get(CV_CAP_PROP_FRAME_WIDTH);
				m_h = m_capture.get(CV_CAP_PROP_FRAME_HEIGHT);
				m_fps = m_capture.get(CV_CAP_PROP_FPS);
				qDebug() << "Cam open: " << m_capture.isOpened() << " w/h:" << m_w << "/" << m_h << " fps:" << m_fps;
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
				m_recording = vCoder->toggle(m_w, m_h, m_fps);

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

#if HAS_PYLON
		/*********************************************************/
		class ImageStream3PylonCamera : public ImageStream {
		public:
			explicit ImageStream3PylonCamera(Config* cfg, CameraConfiguration conf)
				: ImageStream(0, cfg)
				, m_camera(getPylonDevice(conf._selector.index), Pylon::Cleanup_Delete)
			{
				m_w =  conf._width == -1 ? _cfg->CameraWidth : conf._width;
				m_h = conf._height == -1 ? _cfg->CameraHeight : conf._height;
				m_fps = conf._fps == -1 ? _cfg->RecordFPS : conf._fps;

				m_camera.MaxNumBuffer = 1;
				m_camera.Open();

				if (!m_camera.IsOpen()) {
					qWarning() << "Unable to open camera!";
					throw device_open_error("Error loading camera");
				}

				GenApi::CBooleanPtr(m_camera.GetNodeMap().GetNode("AcquisitionFrameRateEnable"))->SetValue(1);

				setFrameRate(m_fps);
				auto const actual_fps = getFrameRate();
				qInfo() << "Actual framerate:" << actual_fps;
				if (std::abs(actual_fps - m_fps) > 0.1) {
					throw device_open_error("Error setting framerate");
				}

				qDebug() << "\nStarting to record on camera " << m_camera.GetDeviceInfo().GetFriendlyName();
				m_camera.StartGrabbing(Pylon::GrabStrategy_LatestImages, Pylon::GrabLoop_ProvidedByUser);
				nextFrame_impl();

				m_recording = false;
				m_encoder = std::make_shared<VideoCoder>(m_fps, _cfg);
			}
			GuiParam::MediaType type() const override
			{
				return GuiParam::MediaType::Camera;
			}

			size_t numFrames() const override
			{
				return (std::numeric_limits<short>::max)();
			}

			bool toggleRecord() override
			{
				if (!m_camera.IsOpen()) {
					return false;
				}
				m_recording = m_encoder->toggle(m_w, m_h, m_fps);

				return m_recording;
			}

			double fps() const override
			{
				return m_fps;
			}

			std::string currentFilename() const override
			{
				return std::string(m_camera.GetDeviceInfo().GetFriendlyName());
			}

		private:

			bool nextFrame_impl() override
			{
				Pylon::CGrabResultPtr grabbed;
				for (auto i = 0; i < m_frame_stride; ++i)
					m_camera.RetrieveResult(100, grabbed, Pylon::TimeoutHandling_Return);

				if (!grabbed->GrabSucceeded()) {
					qCritical() << "Unable to grab frame:" << grabbed->GetErrorDescription();
					return false;
				}

				auto view = toOpenCV(grabbed);
				auto scaled = std::make_shared<cv::Mat>();
				cv::resize(view, *scaled, cv::Size{m_w, m_h});
				if (scaled->type() == CV_8UC1)
            		cv::cvtColor(*scaled, *scaled, cv::COLOR_GRAY2BGR);
				set_current_frame(scaled);
				if (m_recording && m_encoder)
					m_encoder->add(scaled);

				return !scaled->empty();
			}

			bool setFrameNumber_impl(size_t) override
			{
				return false;
			}

			double getFrameRate()
			{
				if (m_camera.IsUsb()) {
					return GenApi::CFloatPtr(m_camera.GetNodeMap().GetNode("AcquisitionFrameRate"))->GetValue();
				}

				throw std::logic_error("Unsupported device class");
			}

			void setFrameRate(double value)
			{
				if (m_camera.IsUsb()) {
					GenApi::CFloatPtr(m_camera.GetNodeMap().GetNode("AcquisitionFrameRate")) ->SetValue(value);
				} else {
					throw std::logic_error("Unsupported camera type");
				}
			}

			Pylon::PylonAutoInitTerm m_pylon;
			Pylon::CInstantCamera m_camera;
			double m_fps;
			int m_w;
			int m_h;
			bool m_recording;

			std::shared_ptr<VideoCoder> m_encoder;
		};
#endif

		/*********************************************************/


		std::shared_ptr<ImageStream> make_ImageStream3NoMedia() {
			return std::make_shared<ImageStream3NoMedia>();
		}

		std::shared_ptr<ImageStream> make_ImageStream3Pictures(Config *cfg, 
			std::vector<boost::filesystem::path> filenames) {
			return std::make_shared<ImageStream3Pictures>(cfg, std::move(filenames));
		}

		std::shared_ptr<ImageStream> make_ImageStream3Video(Config *cfg, const std::vector<boost::filesystem::path> &files) {
			try {
				return std::make_shared<ImageStream3Video>(cfg, files);
			}
			catch (const video_open_error &) {
				return make_ImageStream3NoMedia();
			}
		}

		std::shared_ptr<ImageStream> make_ImageStream3Camera(Config *cfg, CameraConfiguration conf) {
			try {
				switch (conf._selector.type) {
				case CameraType::OpenCV:
					return std::make_shared<ImageStream3OpenCVCamera>(cfg, conf);
#if HAS_PYLON
				case CameraType::Pylon:
					return std::make_shared<ImageStream3PylonCamera>(cfg, conf);
#endif
				default:
					throw std::logic_error("Missing image stream implementation");
				}
			}
			catch (const device_open_error &) {
				return make_ImageStream3NoMedia();
			}
		}

	}
}
