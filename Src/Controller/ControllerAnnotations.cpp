#include "ControllerAnnotations.h"
#include "View/MainWindow.h"
#include "Controller/ControllerGraphicScene.h"
#include "Controller/ControllerPlayer.h"
#include "Controller/ControllerTrackedComponentCore.h"
#include "Model/MediaPlayerStateMachine/MediaPlayerStateMachine.h"
#include "Model/Annotations.h"
#include "View/AnnotationsView.h"
#include "Model/MediaPlayerStateMachine/PlayerParameters.h"

#include <QGuiApplication>

#include <cmath>
#include <limits>
#include <iostream>

ControllerAnnotations::~ControllerAnnotations()
{
	delete getModel();
	delete getView();
}

void ControllerAnnotations::cleanup() {
	// Delete the model to trigger serialization
	delete getModel();
}

void ControllerAnnotations::reset(std::string filepath)
{
	// Replace the model with a fresh one.
	delete getModel();
	createModel(filepath);
	connectModelToController();
}

ControllerAnnotations::ControllerAnnotations(QObject* parent, IBioTrackerContext* context, ENUMS::CONTROLLERTYPE ctr) :
    IControllerCfg(parent, context, ctr) {
}

void ControllerAnnotations::connectControllerToController() {
	{
		IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);
		auto viewController = qobject_cast<ControllerGraphicScene*>(ctr);
		auto view = dynamic_cast<GraphicsView*> (viewController->getView());
		view->addGraphicsItem(static_cast<AnnotationsView*>(getView()));

		QObject::connect(view, &GraphicsView::onMousePressEvent, this, &ControllerAnnotations::mousePressEvent, Qt::DirectConnection);
		QObject::connect(view, &GraphicsView::onMouseReleaseEvent, this, &ControllerAnnotations::mouseReleaseEvent, Qt::DirectConnection);
		QObject::connect(view, &GraphicsView::onMouseMoveEvent, this, &ControllerAnnotations::mouseMoveEvent, Qt::DirectConnection);
		//QObject::connect(view, &GraphicsView::onKeyReleaseEvent, this, &ControllerAnnotations::keyReleaseEvent, Qt::DirectConnection);
		QObject::connect(view, &GraphicsView::onKeyPressEvent, this, &ControllerAnnotations::keyPressEvent, Qt::DirectConnection);


		QWidget *viewport = view->viewport();
		QObject::connect(this, SIGNAL(onRepaintRequired()), viewport, SLOT(repaint()));
	}

	{
		IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::PLAYER);
		auto mediaPlayerController = dynamic_cast<ControllerPlayer*> (ctr);
		assert(mediaPlayerController);
		MediaPlayer *player = dynamic_cast<MediaPlayer*> (mediaPlayerController->getModel());
		assert(player);
		QObject::connect(player, &MediaPlayer::fwdPlayerParameters, this, &ControllerAnnotations::setPlayerParameters);
	}
}

void ControllerAnnotations::createModel(std::string filepath) {
	m_Model = new Annotations(filepath);
}

void ControllerAnnotations::createView() {
	assert(m_Model);
	m_View = new AnnotationsView(this, m_Model);
}

void ControllerAnnotations::connectModelToController() {
	auto model = static_cast<Annotations*>(getModel());
	auto view = static_cast<AnnotationsView*>(getView());
	view->setNewModel(model);
}

void ControllerAnnotations::updateView()
{
	auto view = static_cast<AnnotationsView*>(getView());
	view->prepareUpdate();
	emit(onRepaintRequired());
}

void ControllerAnnotations::keyPressEvent(QKeyEvent *event)
{
	auto model = static_cast<Annotations*>(getModel());
	actionQueued = ActionQueued::None;
	bool handled = true;
	if (event->modifiers()&Qt::AltModifier) {
		switch (event->key())
		{
		case Qt::Key::Key_A:
			actionQueued = ActionQueued::CreateArrow;
			break;
		case Qt::Key::Key_L:
			actionQueued = ActionQueued::CreateLabel;
			break;
		case Qt::Key::Key_R:
			actionQueued = ActionQueued::CreateRect;
			break;
		case Qt::Key::Key_E:
			actionQueued = ActionQueued::CreateEllipse;
			break;
		case Qt::Key::Key_Delete:
			// Remove existing selected annotation at current frame.
			if (model->removeSelection())
			{
				updateView();
			}
			else
				handled = false;
			break;
		case Qt::Key::Key_Q:
			if (!model->updateSelectionStartFrame())
				handled = false;
			break;
		case Qt::Key::Key_W:
			if (!model->updateSelectionEndFrame())
				handled = false;
			break;
		default:
			handled = false;
			break;
		}
	}

	
	if (handled)
		event->accept();
}

void ControllerAnnotations::mousePressEvent(QMouseEvent *event, const QPoint &pos)
{
	// left-click: add annotation in pos or try start dragging annotation
	if (event->button() == Qt::LeftButton) {
		auto model = static_cast<Annotations*>(getModel());
		bool handled = true;

		switch (actionQueued)
		{
		case ActionQueued::CreateArrow:
			model->startArrow(pos, model->getCurrentFrame());
			break;
		case ActionQueued::CreateLabel:
			model->startLabel(pos, model->getCurrentFrame());
			break;
		case ActionQueued::CreateRect:
			model->startRect(pos, model->getCurrentFrame());
			break;
		case ActionQueued::CreateEllipse:
			model->startEllipse(pos, model->getCurrentFrame());
			break;
		default:
			if (model->tryStartDragging(pos)) {
				updateView();
			}
			else {
				handled = false;
			}
			updateView();
			break;
		}
		if (handled)
			event->accept();
		actionQueued = ActionQueued::None;
	}

	// right-click: set text for annotation in pos 
	else if (event->button() == Qt::RightButton) {
		auto model = static_cast<Annotations*>(getModel());
		bool handled = true;

		if (model->trySetText(pos)) {
			updateView();
		}
		else {
			handled = false;
		}
		if (handled) {
			event->accept();
		}

	}
	
}

Annotations::TrackedPoint ControllerAnnotations::snapToTrajectory(const QPoint &originalPoint)
{
	auto model = static_cast<Annotations*>(getModel());
	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE);
	auto trackedComponentCoreController = qobject_cast<ControllerTrackedComponentCore*>(ctr);
	auto trackedTrajectoryModel = dynamic_cast<IModelTrackedTrajectory *>(trackedComponentCoreController->getModel());

	if (trackedTrajectoryModel && QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier))
	{
		auto minDistance = std::numeric_limits<float>::infinity();
		QPoint closestPoint {0, 0};
		int closestTrackID { 0 };
		auto allChildren = trackedTrajectoryModel->getChildNodes();

		for (const auto &childNode : allChildren)
		{
			// Top level nodes are likely trajectories.
			const auto childTrajectory = dynamic_cast<IModelTrackedTrajectory *> (childNode);
			// Not a trajectory? Don't know how to handle.
			if (childTrajectory == nullptr)
				continue;
			
			for (size_t i = 0; i < childTrajectory->size(); ++i)
			{
				if (i != model->getCurrentFrame())
					continue;
				const auto &childComponent = childTrajectory->getChild(i);
				const auto point = dynamic_cast<IModelComponentEuclidian2D*> (childComponent);
				if (point == nullptr)
					continue;
				float distance = std::sqrt(std::pow(point->getXpx() - originalPoint.x(), 2) + std::pow(point->getYpx() - originalPoint.y(), 2));
				if (distance < minDistance)
				{
					minDistance = distance;
					closestPoint = QPoint(static_cast<int>(point->getXpx()), static_cast<int>(point->getYpx()));
					closestTrackID = childTrajectory->getId();
				}
			}
		}

		if (minDistance < 100.0f)
		{
			return Annotations::TrackedPoint(closestPoint, closestTrackID);
		}
	}
	return Annotations::TrackedPoint(originalPoint);
}

void ControllerAnnotations::mouseReleaseEvent(QMouseEvent*event, const QPoint &pos)
{
	auto model = static_cast<Annotations*>(getModel());

	Annotations::TrackedPoint trackedPoint = snapToTrajectory(pos);

	if ((event->button() == Qt::LeftButton) && (model->endAnnotation(trackedPoint) || model->updateAnnotation(trackedPoint)))
	{
		updateView();
		event->accept();
	}
}

void ControllerAnnotations::mouseMoveEvent(QMouseEvent*event, const QPoint &pos)
{
	auto model = static_cast<Annotations*>(getModel());

	Annotations::TrackedPoint trackedPoint = snapToTrajectory(pos);

	if ((event->buttons() & Qt::LeftButton) &&  model->updateAnnotation(trackedPoint))
	{
		updateView();
		event->accept();
	}
}

void ControllerAnnotations::setPlayerParameters(playerParameters* parameters)
{
	auto model = static_cast<Annotations*>(getModel());
	model->setCurrentFrame(parameters->m_CurrentFrameNumber);

	IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::TRACKEDCOMPONENTCORE);
	auto trackedComponentCoreController = qobject_cast<ControllerTrackedComponentCore*>(ctr);
	auto trackedTrajectoryModel = dynamic_cast<IModelTrackedTrajectory *>(trackedComponentCoreController->getModel());
	if (trackedTrajectoryModel != nullptr)
		model->updateTrackedAnnotations(trackedTrajectoryModel->getChildNodes());
	updateView();
}

void ControllerAnnotations::receiveAddLabelAnnotation(){
	actionQueued = ActionQueued::CreateLabel;
}
void ControllerAnnotations::receiveAddRectAnnotation(){
	actionQueued = ActionQueued::CreateRect;
}
void ControllerAnnotations::receiveAddArrowAnnotation(){
	actionQueued = ActionQueued::CreateArrow;
}
void ControllerAnnotations::receiveAddEllipseAnnotation(){
	actionQueued = ActionQueued::CreateEllipse;
}
void ControllerAnnotations::receiveDeleteSelectedAnnotation(){
	auto model = static_cast<Annotations*>(getModel());
	if (model->removeSelection()){
		updateView();
	}
}

void ControllerAnnotations::receiveSetAnnotationColor(QColor color)
{
	AnnotationsView* view = dynamic_cast<AnnotationsView*>(m_View);
	if (view) {
		view->setColor(color);
	}
}

