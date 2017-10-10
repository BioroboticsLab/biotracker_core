#include "Annotations.h"

#include <cmath>
#include <fstream>

Annotations::~Annotations() 
{
	serialize();
}

std::string Annotations::getFilename() const
{
	return filepath + ".annotations.csv";
}

void Annotations::serialize() const
{
	if (!dirty || filepath.empty() || annotations.empty()) return;

	std::ofstream outfile(getFilename(), std::ios_base::out | std::ios_base::trunc);
	if (!outfile.good()) return;

	for (auto & annotation : annotations)
	{
		outfile << annotation.get() << std::endl;
	}

	dirty = !outfile.good();
}

void Annotations::deserialize()
{
	if (filepath.empty()) return;
	std::ifstream infile(getFilename(), std::ios_base::in);
	if (!infile.good()) return;
	annotations.clear();
	// Implements a simple CSV-reader.
	// Could be replaced by library functionality.
	std::string line;
	while (std::getline(infile, line))
	{
		try
		{
			std::string::size_type begin = 0;
			std::queue<std::string> args;
			do
			{
				switch (line[begin])
				{
				case '"':
				{
					const auto end = line.find('"', begin + 1);
					if (end == std::string::npos) throw("parse error");
					args.push(line.substr(begin + 1, end - begin - 1));
					begin = end + 1;
					break;
				}
				case ',':
					++begin;
					break;
				default:
				{
					auto end = line.find('"', begin + 1);
					if (end == std::string::npos)
						end = line.size();
					args.push(line.substr(begin + 1, end - begin - 1));
					begin = end + 1;
					break;
				}
				}
			} while (begin < line.size());

			if (!args.empty())
			{
				const std::string type = args.front();
				args.pop();
				std::shared_ptr<Annotation> annotation;
				if (type == "arrow")
					annotation = std::make_shared<AnnotationArrow>();
				else if (type == "label")
					annotation = std::make_shared<AnnotationLabel>();
				if (annotation)
				{
					annotation->deserializeFrom(args);
					annotations.push_back(annotation);
				}
			}
		}
		catch (...) {}
	}
}

std::ostream& operator<<(std::ostream& stream, const Annotations::Annotation *annotation)
{
	const std::vector<std::string> serialized = annotation->serializeToVector();
	for (auto &str : serialized)
		stream << "\"" << str << "\",";
	return stream;
}

void Annotations::Annotation::drawHandleLocation(QPainter *painter, QPoint pos)
{
	QPen original{ painter->pen() };
	QPen dotted{ original };
	dotted.setStyle(Qt::PenStyle::DotLine);
	dotted.setWidthF(dotted.widthF() / 2.0);
	painter->setPen(dotted);
	const int radius = 20;
	painter->drawEllipse(pos, radius, radius);
	painter->setPen(original);
}

bool Annotations::Annotation::isHandleAtPosition(const QPoint &handle, const QPoint &pos)
{
	QPoint diff = handle - pos;
    const float euclidian = std::sqrt((diff.x() * diff.x()) + (diff.y() * diff.y()));
	return euclidian <= 20.0;
}

void Annotations::Annotation::deserializeFrom(std::queue<std::string> &args)
{
	if (args.size() < 3) return;
	startFrame = std::stoi(args.front()); args.pop();
	const int x = std::stoi(args.front()); args.pop();
	const int y = std::stoi(args.front()); args.pop();
	origin = QPoint(x, y);
}

std::vector<std::string> Annotations::Annotation::serializeToVector() const
{
	return { name(), std::to_string(startFrame), std::to_string(origin.x()), std::to_string(origin.y()) };
}

void Annotations::AnnotationLabel::deserializeFrom(std::queue<std::string> &args)
{
	Annotation::deserializeFrom(args);
	if (args.size() < 0) return;
}

std::vector<std::string> Annotations::AnnotationLabel::serializeToVector() const
{
	auto base = Annotations::Annotation::serializeToVector();
	return base;
}

void Annotations::AnnotationLabel::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) const
{
	QPen original{ painter->pen() };
	QPen thin{ original };
	thin.setWidthF(1.0);
	painter->setPen(thin);
	const int len = 20;
	painter->drawLine(this->origin + QPoint(-len, -len), this->origin + QPoint(+len, +len));
	painter->drawLine(this->origin + QPoint(-len, len), this->origin + QPoint(+len, -len));
	painter->setPen(original);
	
	Annotation::drawHandleLocation(painter, origin);
}

void Annotations::AnnotationArrow::deserializeFrom(std::queue<std::string> &args)
{
	Annotation::deserializeFrom(args);
	if (args.size() < 2) return;
	const int x = std::stoi(args.front()); args.pop();
	const int y = std::stoi(args.front()); args.pop();
	arrowHead = QPoint(x, y);
}

std::vector<std::string> Annotations::AnnotationArrow::serializeToVector() const
{
	auto base = Annotations::Annotation::serializeToVector();
	decltype(base) suffix { std::to_string(arrowHead.x()), std::to_string(arrowHead.y()) };
	base.insert(base.end(), suffix.begin(), suffix.end());
	return base;
}

void Annotations::AnnotationArrow::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) const
{
	painter->drawLine(this->origin, this->arrowHead);
	// Draw the origin slightly bigger.
	{
		painter->save();
		QPen thick{ painter->pen() };
		thick.setWidthF(thick.widthF()* 2.f);
		painter->setPen(thick);
		painter->drawPoint(this->origin);
		painter->restore();
	}
	Annotation::drawHandleLocation(painter, origin);
	Annotation::drawHandleLocation(painter, arrowHead);
}

QPoint *Annotations::AnnotationArrow::getHandleForPosition(const QPoint &pos)
{
	if (isHandleAtPosition(arrowHead, pos)) return &arrowHead;
	return Annotation::getHandleForPosition(pos);
}

void Annotations::startArrow(QPoint origin, size_t currentFrame)
{
	currentAnnotation = std::make_shared<AnnotationArrow>(origin, currentFrame);
}

void Annotations::startLabel(QPoint origin, size_t currentFrame)
{
	currentAnnotation = std::make_shared<AnnotationLabel>(origin, currentFrame);
}

bool Annotations::updateAnnotation(QPoint cursor)
{
	if (currentAnnotation)
	{
		currentAnnotation->onEndAnnotation(cursor);
		return true;
	}
	if (selection)
	{
		*selection.handle = cursor;
		return true;
	}
	return false;
}

bool Annotations::tryStartDragging(QPoint cursor)
{
	selection.reset();
	for (auto &annotation : annotations)
	{
		if (!(selection.handle = annotation->getHandleForPosition(cursor))) continue;
		selection.annotation = annotation;
		return true;
	}
	return false;
}

bool Annotations::endAnnotation(QPoint cursor)
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
		if (annotation.get() == selectedAnnotation)
			iter = annotations.erase(iter);
		else
			++iter;
	}
}
