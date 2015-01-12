#ifndef ColorPatchTracker_H
#define ColorPatchTracker_H

#include "cv.h"
#include "source/tracking/TrackingAlgorithm.h"

class ColorPatchTracker : public TrackingAlgorithm
{
private:
	cv::Mat imgMask;
public:
	ColorPatchTracker();
	ColorPatchTracker(Settings& settings, QWidget *parent);
	~ColorPatchTracker(void);
	void track(ulong frameNumber, cv::Mat & frame) override;
	void paint(cv::Mat& image, View const& view = OriginalView) override;
	void reset() override;
};

#endif
