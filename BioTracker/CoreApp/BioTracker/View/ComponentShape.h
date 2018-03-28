#ifndef COMPONENTSHAPE_H
#define COMPONENTSHAPE_H

#include "QGraphicsObject" 
#include "Interfaces/IView/IViewTrackedComponent.h"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include "qpainter.h"
#include "qgraphicsitem.h"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"
#include "Model/CoreParameter.h"
#include "QTime"
#include "View/Utility/RotationHandle.h"

class ComponentShape : public QGraphicsObject
{
	Q_OBJECT

	public:
		//Constructor
		ComponentShape(QGraphicsObject* parent = 0, IModelTrackedTrajectory* trajectory = 0, int id = -1);
        ~ComponentShape();

		QRectF boundingRect() const override;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
		QPainterPath shape() const override;
		bool advance();

		bool updateAttributes(uint framenumber);
		IModelTrackedTrajectory* getTrajectory();
		void setPermission(std::pair<ENUMS::COREPERMISSIONS, bool> permission);
		int getId();
		bool isSwappable();
		bool isRemovable();
		bool isRotatable();
		QPoint getOldPos();
		void trace();
		void setMembers(CoreParameter* coreParams);

		//public member
		int m_currentFramenumber;
		//to fix a bug when moving; TODO better
		int m_w;
		int m_h;

		QGraphicsRectItem* m_tracingLayer;


	signals:
		void emitRemoveTrajectory(IModelTrackedTrajectory* trajectory);
		void emitRemoveTrackEntity(IModelTrackedTrajectory* trajectory, uint frameNumber);
		void emitMoveElement(IModelTrackedTrajectory* trajectory, QPoint oldPos, QPoint newPos, uint frameNumber, int sizeOfStackToMove);
		void emitToggleFixTrack(IModelTrackedTrajectory* trajectory, bool toggle);
		void emitEntityRotation(IModelTrackedTrajectory* trajectory, double oldAngleDeg, double newAngleDeg, uint frameNumber);
        void emitGoToFrame(int frame);
		void broadcastMove();

	public Q_SLOTS:
		//context menu actions
		void changeBrushColor();
		void changePenColor();
		void changeBrushColor(QColor color);
		void changePenColor(QColor color);
		bool removeTrack();
		bool removeTrackEntity();
		void markShape(int penwidth = 0);
		void unmarkShape();
		void toggleFixTrack();
		void createInfoWindow();
		void setObjectNameContext(QString name);
		void morphIntoRect();
		void morphIntoEllipse();
		void morphIntoPoint();



		//Tracing
		void receiveTracingLength(int tracingLength);
		void receiveTracingStyle(QString style);
		void receiveTracingSteps(int steps);
		void receiveTracingTimeDegradation(QString timeDegradation);
		void receiveTracerProportions(float proportion);
		void receiveTracerOrientationLine(bool toggle);
		void receiveTracerFrameNumber(bool toggle);
		//Visual
		void receiveAntialiasing(bool toggle);
		void receiveTransparency(int alpha);
		//Dimensions
		void receiveDimensions(int width, int height);
		void receiveHeight(int height);
		void receiveWidth(int width);
		void setDimensionsToDefault();
		void receiveToggleOrientationLine(bool toggle);
		void receiveShowId(bool toggle);
		void receiveShapeRotation(double angle, bool rotateEntity);
		//Ignore zoom
		void receiveIgnoreZoom(bool toggle);


	protected:
		QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

		void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
		void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

		double constrainAngle(double x);

	private:

		//Member
		QGraphicsObject *m_parent;
		//permissions
		std::map<ENUMS::COREPERMISSIONS, bool> m_permissions;
		bool m_pMovable;
		bool m_pRemovable;
		bool m_pSwappable;
		bool m_pRotatable;
		//
		bool m_fixed;
		bool m_antialiasing;
		IModelTrackedTrajectory* m_trajectory;
		QList<QPolygonF> m_polygons;
		int m_id;
		int m_z;
		int m_wDefault = 20;
		int m_hDefault = 20;
		bool m_useDefaultDimensions;
		float m_rotation;
		QColor m_penColor;
		QColor m_penColorLast;
		QColor m_brushColor;
		int m_transparency;
		bool m_marked;
		int m_penWidth;
		Qt::PenStyle m_penStyle;
		Qt::PenStyle m_penStylePrev;
		bool m_dragged;
		QTime m_mousePressTime;
		QPoint m_mousePressPos;
		QString m_tracingStyle;
		int m_tracingLength;
		int m_tracingSteps;
		QString m_tracingTimeDegradation;
		float m_tracerProportions;
		bool m_tracingOrientationLine;
		bool m_tracerFrameNumber;
		QLineF m_rotationLine;
		RotationHandle* m_rotationHandle;
		QGraphicsRectItem* m_rotationHandleLayer;
		bool m_orientationLine;
		bool m_showId;
		bool m_trajectoryWasActiveOnce;
		QPoint m_oldPos;
};


#endif // COMPONENTSHAPE_H
