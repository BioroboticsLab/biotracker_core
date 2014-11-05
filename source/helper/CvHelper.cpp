#include "CvHelper.h"

#include "source/helper/StringHelper.h"

#include <sstream>


CvPoint CvHelper::subtractTwoCvPoints(CvPoint a, CvPoint b)
{
	return cvPoint(a.x - b.x, a.y - b.y);
}

CvPoint CvHelper::addTwoCvPoints(CvPoint a, CvPoint b)
{
	return cvPoint(a.x + b.x, a.y + b.y);
}

CvPoint CvHelper::multCvPoint(double scalar, CvPoint p)
{
	return cvPoint(scalar * p.x, scalar * p.y);
}

QPointF CvHelper::norm(double x, double y)
{
	double distance = qSqrt(x * x + y * y);

	if (distance == 0) {
		return QPointF(0, 0);
	}
	return QPointF(x / distance, y / distance);
}

QPointF CvHelper::norm(QPoint p)
{
	return CvHelper::norm(static_cast<double>(p.x()), static_cast<double>(p.y()));
}

QPointF CvHelper::norm(QPointF p)
{
	return CvHelper::norm(p.x(), p.y());
}

double CvHelper::getDistance(double x1, double y1, double x2, double y2)
{
	return qSqrt( ( (x1 - x2) * (x1 - x2) )  + ( (y1 - y2) * (y1 - y2) ) );
}

double CvHelper::getSqDistance(double x1, double y1, double x2, double y2)
{
	return ( (x1 - x2) * (x1 - x2) )  + ( (y1 - y2) * (y1 - y2) );
}

double CvHelper::getDistance(QPoint p1, QPoint p2)
{
  return getDistance(static_cast<double>(p1.x()), static_cast<double>(p1.y()), static_cast<double>(p2.x()), static_cast<double>(p2.y()));
}

double CvHelper::getDistance(QPointF p1, QPointF p2)
{
	return getDistance(p1.x(), p1.y(), p2.x(), p2.y());
}

double CvHelper::getDistance(cv::Point2f p1, cv::Point2f p2)
{
	return getDistance(p1.x, p1.y, p2.x, p2.y);
}

double CvHelper::getDistance(cv::Point p1, cv::Point p2)
{
  return sqrt(static_cast<double>((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y)));
}

double CvHelper::getSqDistance(cv::Point2f p1, cv::Point2f p2)
{
	return getSqDistance(p1.x, p1.y, p2.x, p2.y);
}

double CvHelper::orientation(cv::Point2f front, cv::Point2f back)
{
	cv::Point2f diff = front - back;	
	//return qAtan2(diff.x, diff.y);

	// need to check the origin of coorindiates
	return qAtan2(diff.x, diff.y) + CV_PI / 2.0;
}

double CvHelper::orientation(QPointF front, QPointF back)
{
	//QPointF diff = front - back;
	//return qAtan2(diff.x(), diff.y());
	return orientation(cv::Point2f(front.x(),front.y()),cv::Point2f(back.x(),back.y()));
}

float CvHelper::angleDifference(float alpha, float beta)
{
	float abs_diff = std::abs(alpha - beta);
	if (abs_diff > CV_PI)
		return (2 * CV_PI - abs_diff);
	else return abs_diff;
}

float CvHelper::findMin(float n1, float n2, float n3) {	
	return (n1 < n2 && n1 < n3) ? n1 : (n2 < n3 ? n2 : n3);
}

float CvHelper::findMax(float n1, float n2, float n3) {
	return (n1 > n2 && n1 > n3) ? n1 : (n2 > n3 ? n2 : n3);
}

float CvHelper::normalDist(float x, float mean, float variance) {
	return 1.0 / (variance *qSqrt(2*CV_PI)) * qExp(- ((x - mean) * (x - mean)) / (2 * variance * variance));
}

float CvHelper::sigmoid(float x, float shrink) {
	return (2.0 / (1.0 + qExp(-x * shrink)) - 1.0);
}

float CvHelper::sigmoidAbsInv(float x, float shrink) {
	return 1.0 - qAbs((2.0 / (1.0 + qExp(-x * shrink)) - 1.0));
}

double CvHelper::getAngleDifference(double dirToTargetAsRad, double currOrientationAsRad) {
	/*double angleDiff = dirToTargetAsRad - currOrientationAsRad + CV_PI / 2.0;
	while (angleDiff < -CV_PI) angleDiff += 2 * CV_PI;
	while (angleDiff > CV_PI) angleDiff -= 2 * CV_PI;
	return angleDiff;*/

	double a = dirToTargetAsRad - currOrientationAsRad;
	a += (a > CV_PI) ? -(2*CV_PI) : (a < -CV_PI) ? (2*CV_PI) : 0;
	return a;
}

//double CvHelper::getAngleToTarget(cv::Point2f A, cv::Point2f B) {
//	double ab = A.x*B.x + A.y*B.y;
//	double a = qSqrt(A.x*A.x + A.y*A.y);
//	double b = qSqrt(B.x*B.x + B.y*B.y);
//	double cosT = ab / (a*b);
//	return qAcos(cosT);
//}

double CvHelper::getAngleToTarget(cv::Point2f currentPos, cv::Point2f targetPos) {
	return qAtan2(targetPos.x - currentPos.x, targetPos.y - currentPos.y);
}

std::deque<cv::Point2f> CvHelper::convertMat2Point2fDeque(cv::Mat mat)
{
	std::deque<cv::Point2f> point2fS;

	if(mat.cols != 2)
		return point2fS;

	for (int row = 0; row < mat.rows; row++)
	{
		float x = mat.at<float>(row,0);
		float y = mat.at<float>(row,1);
		point2fS.push_back(cv::Point2f(x,y));
	}

	return point2fS;
}

cv::Mat CvHelper::convertPoint2fDeque2Mat(const std::deque<cv::Point2f> &points)
{
	cv::Mat mat;

	if(points.empty())
		return mat;

	mat = cv::Mat(points.size(), 2, CV_32F);

	for (size_t i = 0; i < points.size(); ++i)
	{
		mat.at<float>(i,0)= points[i].x;
		mat.at<float>(i,1)= points[i].y;
	}
	
	return mat;
}

QList<std::deque<cv::Point2f>> CvHelper::convertMatList2Point2fDequeList(const QList<cv::Mat> &mats)
{
	QList<std::deque<cv::Point2f>> pointList;
	for(const auto &mat : mats)
	{
		pointList << CvHelper::convertMat2Point2fDeque(mat);
	}
	return pointList;
}

cv::Point2f CvHelper::getMirrowPoint(cv::Point2f point2Mirror, cv::Point2f pointOfOrigin, float angelAsGrad)
{	
	//Convert angelAsGrad to radian
	float angelAsRadian = (angelAsGrad *  CV_PI / 180.0);

	cv::Mat G = (cv::Mat_<float>(2,2) << cos(2.0 * angelAsRadian), sin(2.0 * angelAsRadian), sin(2.0 * angelAsRadian), -cos(2.0 * angelAsRadian));

	cv::Mat point2MirrorMat(1/*rows*/,2 /* cols */,CV_32F); 
    point2MirrorMat.at<float>(0,0) = point2Mirror.x; 
    point2MirrorMat.at<float>(0,1) = point2Mirror.y;

	cv::Mat pointOfOriginMat(1/*rows*/,2 /* cols */,CV_32F);
	pointOfOriginMat.at<float>(0,0) = pointOfOrigin.x; 
    pointOfOriginMat.at<float>(0,1) = pointOfOrigin.y;

	cv::Mat point2MirrorFromOrigin = point2MirrorMat - pointOfOriginMat;

	cv::Mat point2MirrorMatT; 
    cv::transpose(point2MirrorFromOrigin, point2MirrorMatT);

	cv::Mat mirrowedPointMat = (G * point2MirrorMatT);

	cv::Mat mirrowedPointMatT;
	cv::transpose(mirrowedPointMat, mirrowedPointMatT);

	cv::Mat finalMat = mirrowedPointMatT + pointOfOriginMat;

	return cv::Point2f(finalMat.at<float>(0,0),finalMat.at<float>(0,1));
}

std::deque<cv::Point2f> CvHelper::getMirrowPoints(const std::deque<cv::Point2f> &points2Mirror, cv::Point2f pointOfOrigin, float angelAsGrad)
{
	std::deque<cv::Point2f> mirrowedPoints;
	
	for(const auto &p : points2Mirror)
	{
		mirrowedPoints.push_back(
		  CvHelper::getMirrowPoint(p, pointOfOrigin, angelAsGrad)
		);
	}
	return mirrowedPoints;

}

std::deque<cv::Point2f> CvHelper::getMirrowLine(cv::Point2f pointOfOrigin, float width, float height, float angelAsGrad)
{	
	float angle = angelAsGrad *  CV_PI / 180.0;

	float r = CvHelper::getDistance(pointOfOrigin, cv::Point2f(width, height));

	float xOff1 = pointOfOrigin.x + r * cos(angle);
	float yOff1 = pointOfOrigin.y + r * sin(angle);

	float xOff2 = pointOfOrigin.x - r * cos(angle);
	float yOff2 = pointOfOrigin.y - r * sin(angle);

	cv::Point2f front(xOff2,yOff2);
	cv::Point2f back(xOff1,yOff1);

	std::deque<cv::Point2f> points;

	points.push_back(front);
	points.push_back(back);
	return points;
}


std::vector<cv::Point> CvHelper::convertMat2Vector(cv::Mat mat)
{
	std::vector<cv::Point> value;
	value.reserve(mat.rows);
	for (int i = 0; i < mat.rows; i++)
	{
		value.emplace_back(mat.at<float>(i,0), mat.at<float>(i,1));
	}
	return value;
}

cv::Mat CvHelper::convertVector2Mat(const std::vector<cv::Point> &vect)
{
	cv::Mat mat(vect.size(),2,CV_32F);
	for (size_t i = 0; i < vect.size(); i++)
	{
		mat.at<float>(i,0) = vect[i].x;
		mat.at<float>(i,1) = vect[i].y;
	}
	return mat;
}

float CvHelper::degToRad(float deg)
{
	return deg * CV_PI / 180.0;
}

float CvHelper::radToDeg(float rad)
{
	return rad * 180.0 / CV_PI;
}

int CvHelper::stdStringToInt(const std::string &string)
{
	int numb;
	std::istringstream ( string ) >> numb;
	return numb;
}

std::string CvHelper::convertStdVectorCvPointToStdString(const std::vector<cv::Point> &points)
{
	std::stringstream ss;
	CvHelper::print_sequence(ss, points.cbegin(), points.cend());
	return ss.str();
}

std::string CvHelper::convertCvScalarToStdString(cv::Scalar scalar)
{
	const int r = scalar.val[0];
	const int g = scalar.val[1];
	const int b = scalar.val[2];

	return std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b);
}

std::string CvHelper::getCurrentDatetimeAsStd()
{
	return QDateTime::currentDateTime().toString("_yyMMddThhmmss").toStdString();
}
