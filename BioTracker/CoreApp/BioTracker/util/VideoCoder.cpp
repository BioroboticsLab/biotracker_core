#include "VideoCoder.h"
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif

#define mySleep(x) std::this_thread::sleep_for(std::chrono::milliseconds(x));

void MutexLinkedList::push(std::shared_ptr<ImageBuffer> imbuffer, bool dropFrames) {

	if (dropFrames) {
		while (images.size() > MAXIMUMQUEUE)
			pop();
	}
	else {
		while (images.size() > MAXIMUMQUEUE)
			mySleep(10);
	}

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

void MutexLinkedList::MutexLinkedList::clear() {
	_Access.lock();
	while (images.size()>0) {
		images.pop_front();
	}
	_Access.unlock();
}


//Simple function to get the current size of the buffer in elements.
//Locks the data structure.
int MutexLinkedList::size() {
	int tsize = 0;
	_Access.lock();
	tsize = images.size();
	_Access.unlock();
	return tsize;
}

///////////////////////////////////////////////////////////////////////////////////////////////

void YuvConverter::convert()
{
	int w = inImg.size().width;
	int h = inImg.size().height;
	unsigned char *prtM = inImg.data;
	unsigned int stride = 1;
	//Y
	int x, y;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			//This works but is horribly slow!
			//out0[y * w + x] = inImg.at<cv::Vec3b>(y, x)[0];
			//out1[y * w + x] = inImg.at<cv::Vec3b>(y, x)[1];
			//out2[y * w + x] = inImg.at<cv::Vec3b>(y, x)[2];
			//This works and is quite a bunch faster
			out0[y * w + x] = (uint8_t)(*prtM);
			prtM++;
			out1[y * w + x] = (uint8_t)(*prtM);
			prtM++;
			out2[y * w + x] = (uint8_t)(*prtM);
			prtM++;
		}
	}
}

void YuvConverter::convert420()
{
	int w = inImg.size().width;
	int h = inImg.size().height;
	unsigned char *prtM = inImg.data;
	unsigned int stride = 1;
	//Y
	int x, y;
	for (y = 0; y < (h / 3 * 2); y++) {
		for (x = 0; x < w; x++) {
			unsigned char ch = (uint8_t)(*prtM);
			out0[y * w + x] = ch;
			prtM++;
		}
	}
	/*
	for (y = 0; y < (h / 3 * 1); y++) {
		for (x = 0; x < w/2; x++) {
			out2[y * w + x] = (uint8_t)(*prtM);
			prtM++;
			out1[y * w + x] = (uint8_t)(*prtM);
			prtM++;
		}
	}*/
}

///////////////////////////////////////////////////////////////////////////////////////////////

void Worker::run() {

#ifdef WITH_CUDA
	if (m_nvEncoder) {
		unsigned char *o0 = m_nvEncoder->getYuvChannel(0);
		unsigned char *o1 = m_nvEncoder->getYuvChannel(1);
		unsigned char *o2 = m_nvEncoder->getYuvChannel(2);
		std::shared_ptr<ImageBuffer> mat;
		while (1) {
			while ((mat = ll.pop())->getWidth() == -1) {
				mySleep(10);
				if (m_abort) return;
			}
			if (m_abort) return;

			cv::Mat writeMat;
			cv::cvtColor(*(mat->_img), writeMat, CV_BGR2YUV);//CV_BGR2YUV_I420 //CV_BGR2YUV
			int chans = writeMat.channels();
			YuvConverter yc(writeMat, o0, o1, o2);
			yc.convert();
			m_nvEncoder->encodeNext();
		}
	}
	else
#endif
	{
		std::shared_ptr<ImageBuffer> mat;
		while (1) {
			while ((mat = ll.pop())->getWidth() == -1) {
				mySleep(10);
				if (m_abort) return;
			}
			if (m_abort) return;

			m_vWriter->write(*mat->_img);
		}
		
	}
}

int VideoCoder::toggle(int fps, int w, int h) {

	//Grab the codec from config file
	BioTracker::Core::Settings *set = BioTracker::Util::TypedSingleton<BioTracker::Core::Settings>::getInstance(CORE_CONFIGURATION);
	std::string codecStr = codecList[set->getValueOrDefault<int>(CFG_CODEC, 0)].second;
	m_dropFrames = set->getValueOrDefault<bool>(CFG_DROPFRAMES, CFG_DROPFRAMES_VAL);

	if (!m_recording)
	{
		worker = std::make_shared<Worker>();

		//Check which one to use
		if (codecStr == "X264") {
			int codec = CV_FOURCC('X', '2', '6', '4');
			vWriter = std::make_shared<cv::VideoWriter>(getTimeAndDate("./CameraCapture", ".avi"), codec, fps, CvSize(w, h), 1);
			m_recording = vWriter->isOpened();
			std::cout << "Video is open:" << m_recording << std::endl;
			m_recType = 2;
			int ok = start();
			m_recording = (ok == 1 ? false : true);
		}
		else if (codecStr == "X264GPU") {
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
			m_recType = 1;
			int ok = start();
			free(chr);
			m_recording = (ok == 1 ? false : true);
			std::cout << "Video is open:" << m_recording << std::endl;
#endif
		}
	}
	else {
		m_recording = false;
		if (m_recType == 1)
			stop();
		if (m_recType == 2)
			stop();//vWriter->release();
		vWriter = 0;
	}
	if (!m_recording)
		m_recType = 0;
	return m_recording;
}

void VideoCoder::add(std::shared_ptr<cv::Mat> m, int needsConversion) {
	worker->ll.push(std::make_shared<ImageBuffer>(m, needsConversion), m_dropFrames);
}

int VideoCoder::start() {
#ifdef WITH_CUDA
	if (m_recType == 1) {
		int ok = m_nvEncoder->init();
		if (ok > 0)
			return ok;
		worker->m_nvEncoder = m_nvEncoder;
		worker->start();
	}
	else
#endif
		if (m_recType == 2) {
			worker->m_vWriter = vWriter;
			worker->start();
		}

	return 0;
}

void VideoCoder::stop() {
	if (m_recType > 0) {
		worker->m_abort = true;
		worker->wait();
#ifdef WITH_CUDA
		if (m_nvEncoder)
			m_nvEncoder->close();
#endif
		if (vWriter)
			vWriter->release();
		worker->ll.clear();
	}
}



