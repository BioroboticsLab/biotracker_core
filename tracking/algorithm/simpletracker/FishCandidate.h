#ifndef FISH_CANDIDATE_H_
#define FISH_CANDIDATE_H_

#include <opencv.hpp>

#include "TrackedFish.h"

class FishCandidate	: public TrackedFish
{
public:
	FishCandidate(void);
	virtual ~FishCandidate(void);
	void increaseScore();
	void decreaseScore();
	int score() const;
	bool operator==(const FishCandidate& other) const;
private:
	int _score;
};

#endif
