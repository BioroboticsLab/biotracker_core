#include "util/misc.h"

std::string getTimeAndDate(std::string prefix, std::string suffix)
{
	time_t  timetitle;
	std::string timeInfo = prefix;
	time(&timetitle);
	std::string t = ctime(&timetitle);
	for (int i = 0; i < t.size(); i++)
		if (t[i] == ' ' || t[i] == ':')
			t[i] = '_';
	timeInfo += t.substr(0, t.size() - 1);
	timeInfo += suffix;
	return timeInfo;
}