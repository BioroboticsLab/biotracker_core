#include "BlobsDetector.h"
#include "source/helper/CvHelper.h"


BlobsDetector::BlobsDetector(Rectification &rectification, Settings &setting) : 
	IDetector<BlobPose>(rectification,setting),
	_mask(nullptr)
{}


void BlobsDetector::filterBlobsBySize(CBlobResult& blobs)
{
	// blobs smaller than the provided blob size
	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_GREATER_OR_EQUAL, _settings.getValueOfParam<double>(TRACKERPARAM::MIN_BLOB_SIZE));
	// blobs bigger than the provided blob size
	blobs.Filter( blobs, B_INCLUDE, CBlobGetArea(), B_LESS_OR_EQUAL,  _settings.getValueOfParam<double>(TRACKERPARAM::MAX_BLOB_SIZE));
}

std::vector<BlobPose> BlobsDetector::findBlobs(const cv::Mat& binImage)
{
	std::vector<BlobPose> blobPoses;

	IplImage iplBinImage = binImage;

	CBlob *currentBlob;
	CBlobResult blobs(&iplBinImage, (_mask ? &IplImage(*_mask) : nullptr), 0);

	// filter the blobs by size criteria
	filterBlobsBySize(blobs);	
	
	for (int i = 0; i < blobs.GetNumBlobs(); i++)
	{
		currentBlob = blobs.GetBlob(i);

		// gets blob center
		int x = currentBlob->GetEllipse().center.x;
		int y = currentBlob->GetEllipse().center.y;
		cv::Point blobPose_px = cv::Point(x, y);		

		// apply homography
		cv::Point2f blobPose_cm = _rectification.pxToCm(blobPose_px);

		// ignore blobs outside the tracking area
		if (!_rectification.inArea(blobPose_cm))
			continue;

		float blobPose_angle = currentBlob->GetEllipse().angle;
		float blobPose_width =  currentBlob->GetEllipse().size.width;
		float blobPose_height = currentBlob->GetEllipse().size.height;

		blobPoses.push_back(BlobPose(blobPose_cm, blobPose_px, blobPose_angle, blobPose_width, blobPose_height));
	}

	return blobPoses;
}

std::vector<BlobPose> BlobsDetector::getPoses(cv::Mat binImage)
{	
	return findBlobs(binImage);
}