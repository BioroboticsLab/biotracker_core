#pragma once

#include "Interfaces/IController/IController.h"
#include "Interfaces/IModel/IModelAreaDescriptor.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include "util/types.h"
#include "Model/MediaPlayerStateMachine/PlayerParameters.h"

class ControllerAreaDescriptor : public IController
{
	Q_OBJECT
public:
	ControllerAreaDescriptor(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::AREADESCRIPTOR);

	void triggerUpdateAreaDescriptor();

signals:
	void updateAreaDescriptor(IModelAreaDescriptor *ad);
    void currentVectorDrag(BiotrackerTypes::AreaType vectorType, int id, double x, double y);
    void changeAreaDescriptorType(QString type);

public slots:
	void setRectificationDimensions(double w, double h);
	void setDisplayRectificationDefinition(bool b);
	void setDisplayTrackingAreaDefinition(bool b);
	void setTrackingAreaAsEllipse(bool b);
    void rcvPlayerParameters(playerParameters* parameters);

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
    BiotrackerTypes::AreaType _watchingVerticeType;

	IView *m_ViewApperture;
    int _w = 1;
    int _h = 1;
    bool _visibleApperture = false;
    bool _visibleRectification = false;
    QString _currentFilename = "No Media";
};