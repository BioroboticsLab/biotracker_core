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
	public:
		//Constructor
		ComponentShape(QGraphicsObject* parent = 0, IModelTrackedTrajectory* trajectory = 0, int id = -1);

		QRectF boundingRect() const override;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
		//QPainterPath shape() const override;
		bool advance();

		bool updatePosition(uint framenumber);
		IModelTrackedTrajectory* getTrajectory();

	protected:
		QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

		void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

	private:
		QGraphicsObject *m_parent;
		IModelTrackedTrajectory* m_trajectory;
		int m_id;
		int m_z;
		int m_w;
		int m_h;

};


#endif // COMPONENTSHAPE_H