#pragma once

#include "AreaDescriptor.h"
#include <QBrush>

class EllipseDescriptor : public AreaDescriptor
{
public:
	EllipseDescriptor(IController *controller = 0, IModel *model = 0);
	~EllipseDescriptor();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	QRectF boundingRect() const override;
	void setBrush(QBrush brush) override;


	void setRect(std::vector<cv::Point> rect) override;
	std::vector<cv::Point> getRect() override;

	// IViewTrackedComponent interface
	public Q_SLOTS:
	void getNotified();

protected:
	bool sceneEventFilter(QGraphicsItem * watched, QEvent * event) override;

private:
	void init();
	void updateEllipse();
	bool _isInit;

	std::vector<cv::Point> _v;
	//float _ellipseRotation;

	QGraphicsItem *_watchingDrag;
	int _dragX;
	int _dragY;
	QBrush _brush;

	//Rects
	std::shared_ptr<QGraphicsEllipseItem> _rectification;
	std::shared_ptr<QGraphicsRectItem> _rectificationMarkerOrig;
	std::shared_ptr<QGraphicsRectItem> _rectificationMarkerEnd;
};

