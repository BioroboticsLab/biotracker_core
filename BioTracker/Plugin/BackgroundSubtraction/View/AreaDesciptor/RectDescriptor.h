#pragma once

#include "AreaDescriptor.h"
#include "cv.h"
#include <QBrush>

class RectDescriptor : public AreaDescriptor
{
	Q_OBJECT
public:
	RectDescriptor(QGraphicsItem *parent = 0, IController *controller = 0, IModel *model = 0);
	~RectDescriptor();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	QRectF boundingRect() const override;

	bool inShape(QPoint p);

	void setBrush(QBrush brush) override;
	void setRect(std::vector<cv::Point> rect) override;
	std::vector<cv::Point> getRect() override;
	
	// IViewTrackedComponent interface
public Q_SLOTS:
	void getNotified();

protected:
	bool sceneEventFilter(QGraphicsItem * watched, QEvent * event) override;
	void updateLinePositions();

private:
	void init();
	bool _isInit;

	//bool _isEllipse;
	//float _ellipseRotation;

	QGraphicsItem *_watchingDrag;
	int _dragX;
	int _dragY;

	std::vector<cv::Point> _v;

	//Rects
	std::vector<std::shared_ptr<QGraphicsRectItem>> _rectification;
	std::vector<std::shared_ptr<QGraphicsLineItem>> _rectificationLines;

	QBrush _brush;
	//Ellipses
	//std::shared_ptr<QGraphicsEllipseItem> _ellipseItem;
};

