#ifndef HISTORY_H
#define HISTORY_H
#include "ObjectModel.h"
#include <deque>

using namespace std;
class History
{
public:
	ObjectModel* getHistoryObject(int frameNumber);
	void add(ObjectModel*, int frameNumber);

private:
	deque<ObjectModel> HistoryObjects;
	int id; //object ID 
};  
#endif // HISTORY_H
