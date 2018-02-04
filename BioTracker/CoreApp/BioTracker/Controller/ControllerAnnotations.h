#pragma once

#include "Interfaces/IController/IController.h"
#include "QString"
#include <QMouseEvent>
#include <QKeyEvent>

struct playerParameters;

class ControllerAnnotations : public IController
{
Q_OBJECT
public:
	ControllerAnnotations(QObject* parent = 0, IBioTrackerContext* context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::ANNOTATIONS);
	virtual ~ControllerAnnotations();

	// IController interface
public:
	void connectControllerToController() override;

public Q_SLOTS:
    void reset(std::string filepath);
	void mousePressEvent(QMouseEvent *event, const QPoint &pos);
	void mouseReleaseEvent(QMouseEvent*event, const QPoint &pos);
	void mouseMoveEvent(QMouseEvent*event, const QPoint &pos);
	void keyReleaseEvent(QKeyEvent *event);
	void setPlayerParameters(playerParameters* parameters);

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
Q_SIGNALS:
	void onRepaintRequired();
};
