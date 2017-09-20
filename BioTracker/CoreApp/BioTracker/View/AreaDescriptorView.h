#pragma once
#include "Interfaces/IView/IViewTrackedComponent.h"
#include "AreaDesciptor/RectDescriptor.h"
#include "AreaDesciptor/AreaDescriptor.h"
#include "AreaDesciptor/EllipseDescriptor.h"

class AreaDescriptorView : public IViewTrackedComponent
{
	Q_OBJECT
public:
	AreaDescriptorView(IController *controller = 0, IModel *model = 0);

	// QGraphicsItem interface
public:
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	RectDescriptor* _rectif;
	AreaDescriptor* _tracking;

	// IViewTrackedComponent interface
	public Q_SLOTS:
	void getNotified() override;

	void updateRectification();
	void updateApperture();

	public Q_SIGNAL:
	void emitUpdateCornersChanged(int id, int relX, int relY);

private:
	QRectF _boundingRect;

	
};

