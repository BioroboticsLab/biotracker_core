#include "util/misc.h"

#include <QPoint>             // QPoint
#include <QString>       
#include <QStringList>       

int split(std::string &txt, std::vector<std::string> &strs, char ch)
{
	std::string::size_type pos = txt.find(ch);
	std::string::size_type initialPos = 0;
	strs.clear();

	// Decompose statement
	while (pos != std::string::npos) {
		strs.push_back(txt.substr(initialPos, pos - initialPos));
		initialPos = pos + 1;

		pos = txt.find(ch, initialPos);
	}

	// Add the last one
	strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

	return strs.size();
}

std::string getTimeAndDate(std::string prefix, std::string suffix)
{
	// time_t  timetitle;
	// std::string timeInfo = prefix;
	// time(&timetitle);
	// std::string t = ctime(&timetitle);
	// for (int i = 0; i < t.size(); i++)
	// 	if (t[i] == ' ' || t[i] == ':')
	// 		t[i] = '_';
	// timeInfo += t.substr(0, t.size() - 1);
	// timeInfo += suffix;
	// return timeInfo;




	time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    char t[30];
    strftime(t, 30, "%Y-%m-%dT%H_%M_%S", timeinfo); //

	std::string out = prefix;

	std::string timeStampF(t);

	out += timeStampF;
	out += suffix;

    return out;
}

std::vector<cv::Point> stringToCVPointVec(std::string arena) {
	std::vector<cv::Point> pts;

	QString qarena(arena.c_str());
	QStringList corners = qarena.split(";");
	for (int i = 0; i < corners.size(); i++) {
		QString cur = corners.at(i);
		QStringList p = cur.split(",");
		pts.push_back(cv::Point(p.at(0).toInt(), p.at(1).toInt()));
	}
	return pts;
}


std::vector<cv::Point> QVecToCvVec(std::vector<QPoint> q) {
	std::vector<cv::Point> v;
	for (int i = 0; i < q.size(); i++)
		v.push_back(cv::Point(q[i].x(), q[i].y()));
	return v;
}
std::vector<QPoint> CvVecToQVec(std::vector<cv::Point> q) {
	std::vector<QPoint> v;
	for (int i = 0; i < q.size(); i++)
		v.push_back(QPoint(q[i].x, q[i].y));
	return v;
}

std::string cvPointsToString(std::vector<cv::Point> ac) {

	std::string arena = "";
	for (int i = 0; i < ac.size(); i++) 
		arena += (std::to_string(ac[i].x) + "," + std::to_string(ac[i].y) + ";");
	return arena.substr(0, arena.size() - 1);
}
