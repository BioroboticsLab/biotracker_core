#ifndef FISH_CANDIDATE_H_
#define FISH_CANDIDATE_H_

#include <opencv2/opencv.hpp>

#include "TrackedFish.h"

class FishCandidate	: public TrackedFish
{
public:
	FishCandidate();
	virtual ~FishCandidate() override;
	void increaseScore();
	void decreaseScore();
	int score() const;
	bool operator==(const FishCandidate& other) const;
	bool operator<(const FishCandidate& other) const;
private:
	int _score;
};

#endif
