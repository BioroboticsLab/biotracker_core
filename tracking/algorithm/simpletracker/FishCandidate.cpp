#include "FishCandidate.h"


FishCandidate::FishCandidate(void) : TrackedFish(0, History()), _score(1)
{
}


FishCandidate::~FishCandidate(void)
{
}

void FishCandidate::increaseScore() {
	++_score;
}

void FishCandidate::decreaseScore() {
	_score -= 2;
}

int FishCandidate::score() const {
	return _score;
}

bool FishCandidate::operator==(const FishCandidate& other) const {
	return _last_known_position == other._last_known_position;
}
