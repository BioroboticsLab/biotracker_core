#include "Annotations.h"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include "Interfaces/IModel/IModelTrackedTrajectory.h"

#include <math.h>
#include <fstream>

#include <QDebug>
#include <QFile>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

Annotations::~Annotations() 
{
	serialize();
}

std::string Annotations::getFilename() const
{
	return filepath + ".annotations.json";
}

void Annotations::serialize() const
{
	if (!dirty || filepath.empty()) return;

	QJsonArray serializedAnnotations;
	for (auto & annotation : annotations)
	{
		serializedAnnotations.append(QJsonObject::fromVariantMap(annotation->serializeToMap()));
	}
	QJsonDocument jsonDocument(serializedAnnotations);
	QFile outfile { QString::fromStdString(getFilename()) };
	if (!outfile.open(QFile::WriteOnly))
		return;
	outfile.write(jsonDocument.toJson());
	
	dirty = false;
}

void Annotations::deserialize()
{
	if (filepath.empty()) return;
	QFile infile { QString::fromStdString(getFilename()) };
	if (!infile.open(QFile::ReadOnly)) return;
	const QJsonDocument jsonDocument { QJsonDocument::fromJson(infile.readAll()) };
	const QJsonArray jsonArray(jsonDocument.array());

	for (const auto & serializedAnnotation : jsonArray)
	{
		const auto jsonObject { serializedAnnotation.toObject() };
		const QString type { jsonObject["type"].toString() };

		std::shared_ptr<Annotation> annotation;
		if (type == "arrow")
			annotation = std::make_shared<AnnotationArrow>();
		else if (type == "label")
			annotation = std::make_shared<AnnotationLabel>();
		else if (type == "rect")
			annotation = std::make_shared<AnnotationRect>();
		else if (type == "ellipse")
			annotation = std::make_shared<AnnotationEllipse>();
		if (annotation)
		{
			annotation->deserializeFrom(jsonObject.toVariantMap());
			annotations.push_back(annotation);
		}
	}

	dirty = false;
}

void Annotations::updateTrackedAnnotations(const QList<IModelTrackedComponent*> &trackedComponents)
{
	for (auto &annotation : annotations)
	{
		annotation->updateTrackedPositions(getCurrentFrame(), trackedComponents);
	}
}

void Annotations::TrackedPoint::update(int currentFrameID, const QList<IModelTrackedComponent*> &trackedComponents)
{
	if (!isLinkedToTrack) return;

	for (const auto &trackedComponent : trackedComponents)
	{
		const auto trackedTrajectory = dynamic_cast<IModelTrackedTrajectory *> (trackedComponent);
		if (trackedTrajectory == nullptr) continue;
		if (trackedTrajectory->getId() != trackID) continue;

		const auto &childComponent = trackedTrajectory->getChild(currentFrameID);
		const auto point = dynamic_cast<IModelComponentEuclidian2D*> (childComponent);
		if (point == nullptr) return;

		position = QPoint(static_cast<int>(point->getXpx()), static_cast<int>(point->getYpx()));
		return;
	}
}

void Annotations::Annotation::drawHandleLocation(QPainter *painter, QPoint pos, QString text)
{
	QPen original{ painter->pen() };
	QPen dotted{ original };
	dotted.setStyle(Qt::PenStyle::DotLine);
	dotted.setWidthF(dotted.widthF() / 2.0);
	painter->setPen(dotted);
	const int radius = 20;
	painter->drawEllipse(pos, radius, radius);

	//text slighy other color --> hue + 20
	//so text can be read when annotation is also contrasting with the background
	QPen textPen{ original };
	QColor textColor{ dotted.color() };
	textColor.setHsv(textColor.hsvHue() + 20, textColor.hsvSaturation(), textColor.value());
	textPen.setColor(textColor);
	painter->setPen(textPen);

	//font
	QFont font= painter->font();
	font.setPointSize(15);
	painter->setFont(font);

	//calculate text width to position variably
	QFontMetrics fm(font);
	int width = fm.width(text);
	int height = fm.height();

	//text position under origin handle
	QPoint textPos = pos + QPoint(-width / 2, 20 + height);

	//draw text
	painter->drawText(textPos, text);

	painter->setPen(original);
}

bool Annotations::Annotation::isHandleAtPosition(const QPoint &handle, const QPoint &pos)
{
	QPoint diff = handle - pos;
    const float euclidian = std::sqrt((diff.x() * diff.x()) + (diff.y() * diff.y()));
	return euclidian <= 20.0;
}

void Annotations::Annotation::deserializeFrom(const QMap<QString, QVariant> &map)
{
	text = map.value("comment", text).toString();
	startFrame = map.value("start_frame", static_cast<int>(startFrame)).toInt();
	endFrame = map.value("end_frame", static_cast<int>(endFrame)).toInt();
	origin.position.setY(map.value("origin_x", origin.x()).toInt());
	origin.position.setY(map.value("origin_y", origin.y()).toInt());
	if (map.contains("origin_track_id"))
	{
		origin.isLinkedToTrack = true;
		origin.trackID = map.value("origin_track_id").toInt();
	}
}

QMap<QString, QVariant> Annotations::Annotation::serializeToMap() const
{
	QMap<QString, QVariant> map;
	map["type"] = QString::fromStdString(name());
	map["comment"] = text;
	map["start_frame"] = static_cast<int>(startFrame);
	map["end_frame"] = static_cast<int>(endFrame);

	if (origin.isLinkedToTrack)
	{
		map["origin_track_id"] = origin.trackID;
	}
	else
	{
		map["origin_x"] = origin.x();
		map["origin_y"] = origin.y();
	}
	return map;
}

void Annotations::AnnotationLabel::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) const
{
	QPen original{ painter->pen() };
	QPen thin{ original };
	thin.setWidthF(1.0);
	painter->setPen(thin);
	const int len = 20;
	painter->drawLine(*this->origin + QPoint(-len, -len), *this->origin + QPoint(+len, +len));
	painter->drawLine(*this->origin + QPoint(-len, len), *this->origin + QPoint(+len, -len));

	painter->setPen(original);
	
	Annotation::drawHandleLocation(painter, origin, text);
}

void Annotations::AnnotationArrow::deserializeFrom(const QMap<QString, QVariant> &map)
{
	Annotation::deserializeFrom(map);
	arrowHead.position.setX(map.value("end_x", arrowHead.x()).toInt());
	arrowHead.position.setY(map.value("end_y", arrowHead.y()).toInt());
	if (map.contains("end_track_id"))
	{
		arrowHead.isLinkedToTrack = true;
		arrowHead.trackID = map.value("end_track_id").toInt();
	}
}

QMap<QString, QVariant> Annotations::AnnotationArrow::serializeToMap() const
{
	auto base = Annotations::Annotation::serializeToMap();
	if (arrowHead.isLinkedToTrack)
	{
		base["end_track_id"] = arrowHead.trackID;
	}
	else
	{
		base["end_x"] = arrowHead.x();
		base["end_y"] = arrowHead.y();
	}
	return base;
}

void Annotations::AnnotationArrow::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) const
{
	painter->drawLine(this->origin.getPoint(), this->arrowHead.getPoint());
	// Draw the origin slightly bigger.
	{
		painter->save();
		QPen thick{ painter->pen() };
		thick.setWidthF(thick.widthF()* 2.5f);
		painter->setPen(thick);
		painter->drawPoint(this->origin.getPoint());
		painter->restore();
	}
	// Draw the arrowhead slightly thinner.
	{
		painter->save();
		QPen thin{ painter->pen() };
		thin.setWidthF(thin.widthF()* 0.5f);
		painter->setPen(thin);
		painter->drawPoint(this->arrowHead.getPoint());
		painter->restore();
	}
	Annotation::drawHandleLocation(painter, origin, text );
	Annotation::drawHandleLocation(painter, arrowHead, "");
}

Annotations::TrackedPoint *Annotations::AnnotationArrow::getHandleForPosition(const QPoint &pos)
{
	if (isHandleAtPosition(arrowHead, pos)) return &arrowHead;
	return Annotation::getHandleForPosition(pos);
}

void Annotations::AnnotationArrow::updateTrackedPositions(int currentFrameID, const QList<IModelTrackedComponent*> &trackedComponents)
{
	Annotation::updateTrackedPositions(currentFrameID, trackedComponents);
	arrowHead.update(currentFrameID, trackedComponents);
}

void Annotations::AnnotationRect::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) const
{
	QRect rect{ origin.getPoint(), arrowHead.getPoint() };
	painter->drawRect(rect);
	// Draw the origin slightly bigger.
	{
		painter->save();
		QPen thick{ painter->pen() };
		thick.setWidthF(thick.widthF()* 2.5f);
		painter->setPen(thick);
		painter->drawPoint(origin.getPoint());
		painter->restore();
	}
	// Draw the bottom right point slightly thinner.
	{
		painter->save();
		QPen thin{ painter->pen() };
		thin.setWidthF(thin.widthF()* 0.5f);
		painter->setPen(thin);
		painter->drawPoint(arrowHead.getPoint());
		painter->restore();
	}
	Annotation::drawHandleLocation(painter, origin, text);
	Annotation::drawHandleLocation(painter, arrowHead, "");
}

void Annotations::AnnotationEllipse::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) const
{
	QRect rect{ origin.getPoint(), arrowHead.getPoint() };
	painter->drawEllipse(rect);
	// Draw the origin slightly bigger.
	{
		painter->save();
		QPen thick{ painter->pen() };
		thick.setWidthF(thick.widthF()* 2.5f);
		painter->setPen(thick);
		painter->drawPoint(origin.getPoint());
		painter->restore();
	}
	// Draw the bottom right point slightly thinner.
	{
		painter->save();
		QPen thin{ painter->pen() };
		thin.setWidthF(thin.widthF()* 0.5f);
		painter->setPen(thin);
		painter->drawPoint(arrowHead.getPoint());
		painter->restore();
	}
	Annotation::drawHandleLocation(painter, origin, text);
	Annotation::drawHandleLocation(painter, arrowHead, "");
}

void Annotations::startArrow(QPoint origin, size_t currentFrame)
{
	currentAnnotation = std::make_shared<AnnotationArrow>(origin, currentFrame);
}

void Annotations::startLabel(QPoint origin, size_t currentFrame)
{
	currentAnnotation = std::make_shared<AnnotationLabel>(origin, currentFrame);
}

void Annotations::startRect(QPoint origin, size_t currentFrame)
{
	currentAnnotation = std::make_shared<AnnotationRect>(origin, currentFrame);
}

void Annotations::startEllipse(QPoint origin, size_t currentFrame)
{
	currentAnnotation = std::make_shared<AnnotationEllipse>(origin, currentFrame);
}

bool Annotations::updateAnnotation(TrackedPoint cursor)
{
	if (currentAnnotation)
	{
		currentAnnotation->onEndAnnotation(cursor);
		dirty = true;
		return true;
	}
	if (selection)
	{
		*selection.handle = cursor;
		dirty = true;
		return true;
	}
	return false;
}

bool Annotations::tryStartDragging(QPoint cursor)
{
	selection.reset();
	for (auto &annotation : annotations)
	{
		if (!(selection.handle = annotation->getHandleForPosition(cursor))) {
			continue;
		}
		selection.annotation = annotation;
		return true;
	}
	return false;
}

bool Annotations::trySetText(QPoint cursor) {
	selection.reset();
	for (auto &annotation : annotations)
	{
		if (!(selection.handle = annotation->getHandleForPosition(cursor))) {
			continue;
		}
		selection.annotation = annotation;

		bool ok;
		QString annoText = QInputDialog::getText(Q_NULLPTR, tr("Set annotation text"),
												tr("Annotation text:"), QLineEdit::Normal, annotation->getText(), &ok);
		if (ok) {
			annotation->setText(annoText);
			dirty = true;
		}
		return true;
	}
	return false;
}

bool Annotations::endAnnotation(TrackedPoint cursor)
{
	if (currentAnnotation)
	{
		const bool isValid = currentAnnotation->onEndAnnotation(cursor);
		if (!isValid) return false;

		annotations.push_back(currentAnnotation);
		currentAnnotation.reset();
		dirty = true;
		return true;
	}

	return false;
}

bool Annotations::removeSelection()
{
	if (!selection) return false;
	Annotation *selectedAnnotation{ selection.annotation.lock().get() };
	
	for (auto iter = annotations.begin(); iter != annotations.end();)
	{
		const auto annotation = *iter;
		if (annotation.get() == selectedAnnotation){
			iter = annotations.erase(iter);
			dirty = true;
		}
		else
			++iter;
	}
	return true;
}

bool Annotations::updateSelectionStartFrame()
{
	if (!selection) return false;
	Annotation *selectedAnnotation{ selection.annotation.lock().get() };
	selectedAnnotation->startFrame = getCurrentFrame();
	if (selectedAnnotation->endFrame < selectedAnnotation->startFrame)
		selectedAnnotation->endFrame = selectedAnnotation->startFrame;
	return true;
}

bool Annotations::updateSelectionEndFrame()
{
	if (!selection) return false;
	Annotation *selectedAnnotation{ selection.annotation.lock().get() };
	selectedAnnotation->endFrame = getCurrentFrame();
	if (selectedAnnotation->startFrame > selectedAnnotation->endFrame)
		selectedAnnotation->startFrame = selectedAnnotation->endFrame;
	return true;
}
