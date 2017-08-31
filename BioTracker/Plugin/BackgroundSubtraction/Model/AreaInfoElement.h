#pragma once
#include "Interfaces/IModel/IModel.h"
#include <cv.h>

class AreaInfoElement : public IModel
{
	Q_OBJECT

public:
	AreaInfoElement(int type = 0);
	~AreaInfoElement();

	bool insideElement(cv::Point p);

	void setVertices(std::vector<cv::Point> p) { 
		_v = p; 
		Q_EMIT updatedVertices();
	};
	std::vector<cv::Point> getVertices() { return _v; };
	int _type;

signals:
	void updatedVertices();

private:
	std::vector<cv::Point> _v;
};

