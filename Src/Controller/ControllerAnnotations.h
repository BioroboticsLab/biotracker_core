#pragma once

#include "Model/Annotations.h"
#include "IControllerCfg.h"
#include "QString"
#include <QColor>
#include <QMouseEvent>
#include <QKeyEvent>

struct playerParameters;

/**
* The  ControllerAnnotations inherits IController
* It is the management unit of the annotation component.
* It controls user input -> creates new annotations in model, forwards moves and deletes them
*/
class ControllerAnnotations : public IControllerCfg
{
	Q_OBJECT
public:
	ControllerAnnotations(QObject* parent = 0, IBioTrackerContext* context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::ANNOTATIONS);
	virtual ~ControllerAnnotations();
	void cleanup();

	// IController interface
public:
	void connectControllerToController() override;

	public Q_SLOTS:

	/// 
	void reset(std::string filepath);
	void mousePressEvent(QMouseEvent *event, const QPoint &pos);
	void mouseReleaseEvent(QMouseEvent*event, const QPoint &pos);
	void mouseMoveEvent(QMouseEvent*event, const QPoint &pos);
	void keyPressEvent(QKeyEvent *event);
	void setPlayerParameters(playerParameters* parameters);

	//annotation receivers
	void receiveAddLabelAnnotation();
	void receiveAddRectAnnotation();
	void receiveAddArrowAnnotation();
	void receiveAddEllipseAnnotation();
	void receiveDeleteSelectedAnnotation();
	void receiveSetAnnotationColor(QColor color);

protected:
	void createModel(std::string filepath = "");
	void createModel() override { createModel(""); }
	void createView() override;
	void connectModelToController() override;

	enum class ActionQueued
	{
		None,
		CreateArrow,
		CreateLabel,
		CreateRect,
		CreateEllipse,
	};
	ActionQueued actionQueued{ ActionQueued::None };
	void updateView();
	Annotations::TrackedPoint snapToTrajectory(const QPoint &point);
Q_SIGNALS:
	void onRepaintRequired();
};
