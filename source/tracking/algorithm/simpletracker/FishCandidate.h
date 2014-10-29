#ifndef FISH_CANDIDATE_H_
#define FISH_CANDIDATE_H_

#include "TrackedFish.h"

#include <cereal/access.hpp>
#include <opencv2/opencv.hpp>

class FishCandidate	: public TrackedFish
{
public:
    FishCandidate();
    virtual ~FishCandidate() override {}

    void increaseScore();
    void decreaseScore();
    int score() const;

    bool operator==(const FishCandidate& other) const;
    bool operator<(const FishCandidate& other) const;
private:
    int _score;

    friend class cereal::access;
    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(_last_known_position,
           _age_of_last_known_position,
           _associated_color,
           _score);
    }
};

#endif
