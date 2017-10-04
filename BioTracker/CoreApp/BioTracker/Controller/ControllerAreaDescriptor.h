#pragma once

#include "Interfaces/IController/IController.h"
#include "Interfaces/IModel/IModelAreaDescriptor.h"
#include <QMouseEvent>
#include <QKeyEvent>

class ControllerAreaDescriptor : public IController
{
	Q_OBJECT
public:
	ControllerAreaDescriptor(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::AREADESCRIPTOR);

	void triggerUpdateAreaDescriptor();

signals:
	void updateAreaDescriptor(IModelAreaDescriptor *ad);

public slots:
	void setRectificationDimensions(double w, double h);
	void setDisplayRectificationDefinition(bool b);
	void setDisplayTrackingAreaDefinition(bool b);
	void setTrackingAreaAsEllipse(bool b);

private slots:
	void trackingAreaType(int v);

	void mousePressEvent(QMouseEvent *event, const QPoint &pos);
	void mouseReleaseEvent(QMouseEvent*event, const QPoint &pos);
	void mouseMoveEvent(QMouseEvent*event, const QPoint &pos);
	void keyReleaseEvent(QKeyEvent *event);
	void updateView();

	// IController interface
protected:
	void createModel() override;
	void createView() override;
	void connectModelToController() override;
	void connectControllerToController() override;
private:
	int _watchingVertice;
	int _watchingVerticeType;

	IView *m_ViewApperture;

};