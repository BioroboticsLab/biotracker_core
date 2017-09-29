#pragma once

#include "Interfaces/IView/IViewTrackedComponent.h"
#include <cv.h>

class AreaDescriptor : public IView, public QGraphicsItem
{
public:
    AreaDescriptor(IController *controller, IModel *model) :
		IView(controller, model) {};
	~AreaDescriptor();

	virtual void setBrush(QBrush brush) = 0;
	virtual void setRect(std::vector<cv::Point> rect) = 0;
	virtual std::vector<cv::Point> getRect() = 0;
	virtual void updateRect() = 0;
};

