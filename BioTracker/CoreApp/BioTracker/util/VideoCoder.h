#pragma once


#include <opencv2/opencv.hpp>

#include <qthread.h>
#include <mutex>

#include "util/misc.h"
#ifdef WITH_CUDA
#include "EncoderInterface.h"
#endif

#include "settings/Settings.h"
#include "util/types.h"


class YuvConverter
{
private:
	cv::Mat& inImg;
	unsigned char* out0;
	unsigned char* out1;
	unsigned char* out2;
public:
	YuvConverter(cv::Mat& inputImgage
		, unsigned char* o0
		, unsigned char* o1
		, unsigned char* o2)
		: inImg(inputImgage), out0(o0), out1(o1), out2(o2) {
	}
	void convert();
	void convert420();
};

class ImageBuffer {
public:
	std::shared_ptr<cv::Mat> _img;
	int _needsConversion;

	ImageBuffer(std::shared_ptr<cv::Mat> pimg, int ncv) : _img(pimg), _needsConversion(ncv) {

	}
	ImageBuffer(){}

	int getWidth() {
		if (_img)
			return _img->size().width;
		return -1;
	}

	int getHeight() {
		if (_img)
			return _img->size().height;
		return -1;
	}
};

class MutexLinkedList {
public:
	std::list<std::shared_ptr<ImageBuffer>> images;
	std::mutex _Access;

	void MutexLinkedList::push(std::shared_ptr<ImageBuffer> imbuffer) {
		_Access.lock();

		images.push_back(imbuffer);
		unsigned long s = images.size();
		unsigned long w = imbuffer->getWidth();
		unsigned long h = imbuffer->getHeight();
		//if (s*w*h / 1024 / 1024 > BUFFER_HARDLIMIT) {
		//	std::cout << "ERROR: Buffer exceeds hardlimit (" << BUFFER_HARDLIMIT << " MB). Exiting. " << std::endl;
		//	std::exit(1);
		//}
		_Access.unlock();
	}

	std::shared_ptr<ImageBuffer> MutexLinkedList::pop() {
		_Access.lock();
		if (images.size()>0) {
			std::shared_ptr<ImageBuffer> img = images.front();
			images.pop_front();
			_Access.unlock();
			return img;
		}
		_Access.unlock();

		return std::make_shared<ImageBuffer>();
	}


	//Simple function to get the current size of the buffer in elements.
	//Locks the data structure.
	virtual int size() {
		int tsize = 0;
		_Access.lock();
		tsize = images.size();
		_Access.unlock();
		return tsize;
	}

	MutexLinkedList() {};

	~MutexLinkedList() {};
};

class Worker : public QThread
{
	Q_OBJECT

public:
	MutexLinkedList ll;
	bool m_abort;
#ifdef WITH_CUDA
	std::shared_ptr<EncoderInterface> m_nvEncoder;
#endif

	Worker() {
		m_abort = false;
	};
	void run();

	public slots:

	//void doWork(const QString &parameter);

};

class VideoCoder : public QObject
{
	Q_OBJECT

public:
	VideoCoder() {
		worker = std::make_shared<Worker>();
#ifdef WITH_CUDA
		m_nvEncoder = std::make_shared<EncoderInterface>();
#endif
		m_recType = 0;
		m_recording = false;
	}

	int toggle(int fps, int w, int h) {
		if (!m_recording)
		{
			//Grab the codec from config file
			BioTracker::Core::Settings *set = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
			std::string codecStr = codecList[set->getValueOrDefault<int>(CFG_CODEC, 0)].second;

			//Check which one to use
			if (codecStr=="X264") {
				int codec = CV_FOURCC('X', '2', '6', '4');
				vWriter = std::make_shared<cv::VideoWriter>(getTimeAndDate("./CameraCapture", ".avi"), codec, fps, CvSize(w, h), 1);
				m_recording = vWriter->isOpened();
				std::cout << "Video is open:" << m_recording << std::endl;
				m_recType = 2;
			}
			else if (codecStr=="X264GPU") {
#ifdef WITH_CUDA
				auto cfg = m_nvEncoder->getEncodeConfig();
				cfg->fps = fps;
				cfg->width = w;
				cfg->height = h;
				cfg->codec = NV_ENC_H264;
				cfg->inputFormat = NV_ENC_BUFFER_FORMAT_YUV444;//NV_ENC_BUFFER_FORMAT_NV12;//
				const std::string f = getTimeAndDate("CameraCapture", ".avi");
				char* chr = strdup(f.c_str());
				m_nvEncoder->setOutfile(chr);
				int ok = start();
				free(chr);
				m_recording = (ok == 1 ? false : true);
				m_recType = 1;
				std::cout << "Video is open:" << m_recording << std::endl;
#endif
			}
		}
		else {
			m_recording = false;
			if (m_recType == 1)
				stop();
			if (m_recType == 2)
				vWriter->release();
			vWriter = 0;
		}
		if (!m_recording)
			m_recType = 0;
		return m_recording;
	}

	int start() {
#ifdef WITH_CUDA
		int ok = m_nvEncoder->init();
		if (ok > 0)
			return ok;
		worker->m_nvEncoder = m_nvEncoder;
		worker->start();
#endif
		return 0;
	}

	void add(std::shared_ptr<cv::Mat> m, int needsConversion = 0) {
		if (m_recType == 1)
			worker->ll.push(std::make_shared<ImageBuffer>(m, needsConversion));
		else if (m_recType == 2)
			vWriter->write(*m);
	}

	void stop() {
#ifdef WITH_CUDA
		worker->m_abort = true;
		worker->wait();
		m_nvEncoder->close();
#endif
	}

	~VideoCoder() {
	}
	std::shared_ptr<Worker> worker;

#ifdef WITH_CUDA
	std::shared_ptr<EncoderInterface> m_nvEncoder;
#endif
	std::shared_ptr<cv::VideoWriter> vWriter;
	int m_recType;
	int m_recording;
signals:
	void operate(const QString &);
};

