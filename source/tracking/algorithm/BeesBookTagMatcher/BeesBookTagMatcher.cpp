#include "BeesBookTagMatcher.h"


BeesBookTagMatcher::BeesBookTagMatcher( Settings & settings ) : TrackingAlgorithm( settings )
{
}


BeesBookTagMatcher::~BeesBookTagMatcher(void)
{
}

void BeesBookTagMatcher::track		( std::vector<TrackedObject> & objectList, ulong frameNumber, cv::Mat& frame ){}
void BeesBookTagMatcher::paint		( cv::Mat& image ){}
void BeesBookTagMatcher::reset		(){}