#include "AnnotationsView.h"

#include <limits>
#include <QGraphicsWidget>
#include <util/ScreenHelper.h>

#include "Interfaces/IController/IController.h"

#include "Model/TextureObject.h"
#include "Model/Annotations.h"
#include "View/GraphicsView.h"

AnnotationsView::~AnnotationsView()
{
}

void AnnotationsView::prepareUpdate()
{
	prepareGeometryChange();
}

void AnnotationsView::setColor(QColor color)
{
	_annoColor = color;
}

QRectF AnnotationsView::boundingRect() const
{
	auto model = static_cast<const Annotations*>(getModel());

	QRectF rect{};
	for (auto &annotation : model->annotations)
	{
		rect = rect.united(annotation->boundingRect());
	}

	if (model->currentAnnotation)
		rect = rect.united(model->currentAnnotation->boundingRect());
	return rect;
}


void AnnotationsView::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	setZValue(-1);
	auto model = static_cast<const Annotations*>(getModel());
	const auto currentFrame = model->getCurrentFrame();
	for (auto &annotation : model->annotations)
	{
		// Is the current frame in the annotation's range?
		if ((currentFrame >= annotation->startFrame && currentFrame <= annotation->endFrame)
			|| (currentFrame == annotation->startFrame && annotation->startFrame > annotation->endFrame))
			painter->setPen(QPen(_annoColor, 6, Qt::SolidLine, Qt::RoundCap));
		else
		{
			QColor transparentGray = Qt::gray;
			transparentGray.setAlphaF(0.25);
			painter->setPen(QPen(transparentGray, 6, Qt::SolidLine, Qt::RoundCap));
		}
		annotation->paint(painter, option, widget);
	}

	if (model->currentAnnotation)
	{
		QColor transparentYellow = _annoColor;
		transparentYellow.setAlphaF(0.5);
		painter->setPen(QPen(transparentYellow, 6, Qt::SolidLine, Qt::RoundCap));
		model->currentAnnotation->paint(painter, option, widget);
	}
	if (model->selection)
	{
		painter->setPen(QPen(Qt::red, 6, Qt::SolidLine, Qt::RoundCap));
		Annotations::Annotation::drawHandleLocation(painter, *model->selection.handle, "");
	}
}