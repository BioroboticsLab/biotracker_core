
#ifndef TrackedObject_h
#define TrackedObject_h
#include "History.h"

class TrackedObject
{
public:
	TrackedObject(int id, History his);
private:
	int id;
	History history;
};  
#endif // !TrackedObject_h
