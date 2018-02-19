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
		if (model->tryStartDragging(pos))
			updateView();
		else
			handled = false;
			updateView();
		break;
	}
	if (handled)
		event->accept();
	actionQueued = ActionQueued::None;
}

void ControllerAnnotations::mouseReleaseEvent(QMouseEvent*event, const QPoint &pos)
{
	auto model = static_cast<Annotations*>(getModel());

	if (model->endAnnotation(pos))
	{
		updateView();
		event->accept();
	}
}

void ControllerAnnotations::mouseMoveEvent(QMouseEvent*event, const QPoint &pos)
{
	auto model = static_cast<Annotations*>(getModel());

	if (event->buttons() == Qt::LeftButton &&  model->updateAnnotation(pos))
	{
		updateView();
		event->accept();
	}
}

void ControllerAnnotations::setPlayerParameters(playerParameters* parameters)
{
	auto model = static_cast<Annotations*>(getModel());
	model->setCurrentFrame(parameters->m_CurrentFrameNumber);
	emit(onRepaintRequired());
}

void ControllerAnnotations::receiveAddLabelAnno(){
	actionQueued = ActionQueued::CreateLabel;
}
void ControllerAnnotations::receiveAddRectAnno(){
	actionQueued = ActionQueued::CreateRect;
}
void ControllerAnnotations::receiveAddArrAnno(){
	actionQueued = ActionQueued::CreateArrow;
}
void ControllerAnnotations::receiveAddEllAnno(){
	actionQueued = ActionQueued::CreateEllipse;
}
void ControllerAnnotations::receiveDelSelAnno(){
	auto model = static_cast<Annotations*>(getModel());
	if (model->removeSelection()){
		updateView();
	}
}

