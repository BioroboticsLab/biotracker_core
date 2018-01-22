#ifndef MISC_H
#define MISC_H
#include <string>
#include <time.h>
#include <cv.h>
#include <QPoint>

int split(std::string &txt, std::vector<std::string> &strs, char ch);
std::string getTimeAndDate(std::string prefix, std::string suffix);
std::vector<cv::Point> stringToCVPointVec(std::string arena);
std::vector<cv::Point> QVecToCvVec(std::vector<QPoint> q);
std::string cvPointsToString(std::vector<cv::Point> ac);
std::vector<QPoint> CvVecToQVec(std::vector<cv::Point> q);
#endif