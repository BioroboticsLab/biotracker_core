#include "ControllerAnnotations.h"
#include "View/MainWindow.h"
#include "Controller/ControllerGraphicScene.h"
#include "Controller/ControllerPlayer.h"
#include "Model/MediaPlayerStateMachine/MediaPlayerStateMachine.h"
#include "Model/Annotations.h"
#include "View/AnnotationsView.h"
#include "Model/MediaPlayerStateMachine/PlayerParameters.h"

ControllerAnnotations::~ControllerAnnotations()
{
	delete getModel();
	delete getView();
}

void ControllerAnnotations::reset(std::string filepath)
{
	// Replace the model with a fresh one.
	delete getModel();
	createModel(filepath);
	connectModelToController();
}

ControllerAnnotations::ControllerAnnotations(QObject* parent, IBioTrackerContext* context, ENUMS::CONTROLLERTYPE ctr) :
    IController(parent, context, ctr) {
}

void ControllerAnnotations::connectControllerToController() {
	{
		IController* ctr = m_BioTrackerContext->requestController(ENUMS::CONTROLLERTYPE::GRAPHICSVIEW);
		auto viewController = qobject_cast<ControllerGraphicScene*>(ctr);
		auto view = dynamic_cast<GraphicsView*> (viewController->getView());
		view->addGraphicsItem(static_cast<AnnotationsView*>(getView()));

		QObject::connect(view, &GraphicsView::onMousePressEvent, this, &ControllerAnnotations::mousePressEvent);
		QObject::connect(view, &GraphicsView::onMouseReleaseEvent, this, &ControllerAnnotations::mouseReleaseEvent);
		QObject::connect(view, &GraphicsView::onMouseMoveEvent, this, &ControllerAnnotations::mouseMoveEvent);
		QObject::connect(view, &GraphicsView::onKeyReleaseEvent, this, &ControllerAnnotations::keyReleaseEvent);

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

void ControllerAnnotations::keyReleaseEvent(QKeyEvent *event)
{
	auto model = static_cast<Annotations*>(getModel());
	actionQueued = ActionQueued::None;
	switch (event->key())
	{
	case Qt::Key::Key_A:
		actionQueued = ActionQueued::CreateArrow;
		break;
	case Qt::Key::Key_L:
		actionQueued = ActionQueued::CreateLabel;
		break;
	case Qt::Key::Key_Delete:
		// Remove existing annotations at current frame.
		if (model->removeSelection())
		{
			updateView();
		}
		break;
	default:
		break;
	}
}

void ControllerAnnotations::mousePressEvent(QMouseEvent *event, const QPoint &pos)
{
	auto model = static_cast<Annotations*>(getModel());

	switch (actionQueued)
	{
	case ActionQueued::CreateArrow:
		model->startArrow(pos, model->getCurrentFrame());
		break;
	case ActionQueued::CreateLabel:
		model->startLabel(pos, model->getCurrentFrame());
		break;
	default:
		if (model->tryStartDragging(pos))
			updateView();
		break;
	}
	actionQueued = ActionQueued::None;
}

void ControllerAnnotations::mouseReleaseEvent(QMouseEvent*event, const QPoint &pos)
{
	auto model = static_cast<Annotations*>(getModel());

	if (model->endAnnotation(pos))
	{
		updateView();
	}
}

void ControllerAnnotations::mouseMoveEvent(QMouseEvent*event, const QPoint &pos)
{
	auto model = static_cast<Annotations*>(getModel());

	if (model->updateAnnotation(pos))
	{
		updateView();
	}
}

void ControllerAnnotations::setPlayerParameters(playerParameters* parameters)
{
	auto model = static_cast<Annotations*>(getModel());
	model->setCurrentFrame(parameters->m_CurrentFrameNumber);
	emit(onRepaintRequired());
}
