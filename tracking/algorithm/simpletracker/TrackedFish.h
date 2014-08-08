#ifndef TrackedFish_H
#define TrackedFish_H

#include "trackedObject/TrackedObject.h"
#include "trackedObject/History.h"

class TrackedFish :
	public TrackedObject
{
public:
	TrackedFish(int id, History his);
	~TrackedFish(void);
};

#endif