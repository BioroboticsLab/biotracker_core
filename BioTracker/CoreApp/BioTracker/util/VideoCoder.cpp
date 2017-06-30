#include "VideoCoder.h"


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


void Worker::run() {

#ifdef WITH_CUDA
	unsigned char *o0 = m_nvEncoder->getYuvChannel(0);
	unsigned char *o1 = m_nvEncoder->getYuvChannel(1);
	unsigned char *o2 = m_nvEncoder->getYuvChannel(2);
	std::shared_ptr<ImageBuffer> mat;
	while (1) {
		while ((mat = ll.pop())->getWidth() == -1) {
			Sleep(10);
			if (m_abort) return;
		}

		cv::Mat writeMat;
		if (mat->_needsConversion) {

		}
		cv::cvtColor(*(mat->_img), writeMat, CV_BGR2YUV);//CV_BGR2YUV_I420
		int chans = writeMat.channels();
		YuvConverter yc(writeMat, o0, o1, o2);
		yc.convert();
		m_nvEncoder->encodeNext();
	}
#endif
}
