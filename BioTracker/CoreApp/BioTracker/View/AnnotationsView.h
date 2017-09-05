#pragma once

#include "Interfaces/IView/IView.h"

#include <QPainter>
#include <QGraphicsItem>
#include <cassert>

class GraphicsView;

class AnnotationsView : public IView, public QGraphicsItem
{
public:
	using IView::IView;
	AnnotationsView() = delete;
	virtual ~AnnotationsView();

public:
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;
	void prepareUpdate();
protected:
	QRectF boundingRect() const;

	// IView interface
public:
	void setNewModel(IModel *model) override { setModel(model); };
protected:
	void connectModelView() override {};
};

