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

#define MAXIMUMQUEUE 30

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

    void push(std::shared_ptr<ImageBuffer> imbuffer, bool dropFrames = false);

    std::shared_ptr<ImageBuffer> pop();

    void clear();


	//Simple function to get the current size of the buffer in elements.
	//Locks the data structure.
	virtual int size();

    MutexLinkedList() {}
    ~MutexLinkedList() {}
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
	std::shared_ptr<cv::VideoWriter> m_vWriter;

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
#ifdef WITH_CUDA
		m_nvEncoder = std::make_shared<EncoderInterface>();
#endif
		m_recType = 0;
		m_recording = false;
		m_dropFrames = false;
	}

	~VideoCoder() {
		stop();
	}

	int toggle(int fps, int w, int h);

	void add(std::shared_ptr<cv::Mat> m, int needsConversion = 0);

	int start();
	void stop();
#ifdef WITH_CUDA
	std::shared_ptr<EncoderInterface> m_nvEncoder;
#endif

private:

	std::shared_ptr<Worker> worker;
	std::shared_ptr<cv::VideoWriter> vWriter;
	int m_recType;
	int m_recording;
	bool m_dropFrames;
	int m_qp;
signals:
	void operate(const QString &);
};

