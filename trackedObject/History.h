#ifndef HISTORY_H
#define HISTORY_H
#include "ObjectModel.h"
#include <deque>

using namespace std;
class History
{
public:
	ObjectModel* getHistoryObject();
	void add(ObjectModel*);
	void removeLast();
private:
	deque<ObjectModel> HistoryObjects;
	int id; //object ID 
};  
#endif // HISTORY_H
