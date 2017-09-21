#pragma once

#include "Interfaces/IView/IViewTrackedComponent.h"
#include <cv.h>

class AreaDescriptor : public IViewTrackedComponent
{
	Q_OBJECT
public:
    AreaDescriptor(QGraphicsItem *parent, IController *controller, IModel *model) :
		IViewTrackedComponent(parent, controller, model) {};
	~AreaDescriptor();

	virtual void setBrush(QBrush brush) = 0;
	virtual void setRect(std::vector<cv::Point> rect) = 0;
	virtual std::vector<cv::Point> getRect() = 0;

signals:
	void updatedPoints();
};

