#include "util/misc.h"

#include <QPoint>             // QPoint
#include <QString>       
#include <QStringList>       


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
