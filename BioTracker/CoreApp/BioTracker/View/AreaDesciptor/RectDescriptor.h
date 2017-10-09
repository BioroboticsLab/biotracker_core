#pragma once

#include "AreaDescriptor.h"
#include "cv.h"
#include <QBrush>
#include "util/types.h"
#include <QObject>

class RectDescriptor : public QObject, public AreaDescriptor
{
    Q_OBJECT
public:
	RectDescriptor(IController *controller = 0, IModel *model = 0);
	~RectDescriptor();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	QRectF boundingRect() const override;

	bool inShape(QPoint p);

	void setBrush(QBrush brush) override;
	void setRect(std::vector<cv::Point> rect) override;
	std::vector<cv::Point> getRect() override;

	void updateRect() override;
	
	// IViewTrackedComponent interface
public Q_SLOTS:
	void getNotified();
    void receiveDragUpdate(BiotrackerTypes::AreaType vectorType, int id, double x, double y);

protected:
	bool sceneEventFilter(QGraphicsItem * watched, QEvent * event) override;
	void updateLinePositions();

private:
	void init();
	bool _isInit;

	QGraphicsItem *_watchingDrag;
	int _dragX;
	int _dragY;

	std::vector<cv::Point> _v;

	//Rects
	std::vector<std::shared_ptr<QGraphicsRectItem>> _rectification;
    std::vector<std::shared_ptr<QGraphicsLineItem>> _rectificationLines;
    std::vector<std::shared_ptr<QGraphicsSimpleTextItem>> _rectificationNumbers;

	QBrush _brush;

    BiotrackerTypes::AreaType _dragType;
    int _dragVectorId;
    QPoint _drag;

	// IView interface
public:
	void setNewModel(IModel *model) override { setModel(model); };
protected:
	void connectModelView() override {};
};

