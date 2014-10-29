
#ifndef TrackedObject_h
#define TrackedObject_h
#include "ObjectModel.h"
#include <deque>


class TrackedObject
{
public:
	TrackedObject(int id);
	ObjectModel* getObject(int frameNumber);
	void add(ObjectModel*, int frameNumber);
	void exchange(ObjectModel*, int frameNumber);
	virtual ~TrackedObject() = 0;
private:
	int _id;	
	std::deque<ObjectModel> _objectTimeStamps;
};  
#endif // !TrackedObject_h

	