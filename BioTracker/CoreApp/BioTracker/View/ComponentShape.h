#ifndef COMPONENTSHAPE_H
#define COMPONENTSHAPE_H

#include "QGraphicsObject" 
#include "Interfaces/IView/IViewTrackedComponent.h"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include "qpainter.h"
#include "qgraphicsitem.h"
#include "View/TrackedComponentView.h"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"


class ComponentShape : public QGraphicsObject
{
	Q_OBJECT

	public:
		//Constructor
		ComponentShape(QGraphicsObject* parent = 0, IModelTrackedTrajectory* trajectory = 0, int id = -1);

		QRectF boundingRect() const override;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
		QPainterPath shape() const override;
		bool advance();

		bool updatePosition(uint framenumber);
		IModelTrackedTrajectory* getTrajectory();
		void setPermission(std::pair<ENUMS::COREPERMISSIONS, bool> permission);
		int getId();

	signals:
		void emitRemoveTrajectory(IModelTrackedTrajectory* trajectory);
		void emitMoveElement(IModelTrackedTrajectory* trajectory, QPoint pos);

	public Q_SLOTS:
		void changeBrushColor();
		void changePenColor();
		bool removeShape();
		void markShape(int penwidth = 4);
		void unmarkShape();

	protected:
		//QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

		void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
		//void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

		void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

	private:
		QGraphicsObject *m_parent;
		std::map<ENUMS::COREPERMISSIONS, bool> m_permissions;
		bool m_removable;
		bool m_swappable;
		IModelTrackedTrajectory* m_trajectory;
		int m_id;
		int m_z;
		int m_w;
		int m_h;
		QColor m_penColor;
		QColor m_brushColor;
		bool m_marked;
		int m_penWidth;
		bool m_dragged;
};


#endif // COMPONENTSHAPE_H