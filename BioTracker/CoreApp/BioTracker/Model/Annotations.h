/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg and BioTracker in version 2.
  **
  ****************************************************************************/


#pragma once

#include "Interfaces/IModel/IModel.h"

#include "QString"
#include <QPoint>
#include <QPainter>
#include <QGraphicsItem>

#include <iostream>
#include <vector>
#include <memory>
#include <queue>

/*
	Model to handle annotations, including serialization.
	Annotations will be drawn on top of the video. Their position is defined in image coordinates.
*/
class Annotations : public IModel {
    Q_OBJECT
public:
	Annotations(std::string filepath = "") : filepath(filepath) { deserialize(); }
	virtual ~Annotations();

	/*
		Base annotation class.
		Children must at least support serialization and drawing.
	*/
	struct Annotation
	{
		Annotation(QPoint origin = { 0,0 }, size_t startFrame = 0) : origin(origin), startFrame(startFrame) {}
		virtual ~Annotation() = default;
		// Position in pixels.
		QPoint origin{ 0, 0 };
		size_t startFrame{ 0 };
		// Name that identifies this type of annotation and is used for serialization.
		virtual std::string name() const = 0;
		virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) const = 0;
		// Saving requires writing properties to vector of strings.
		virtual std::vector<std::string> serializeToVector() const;
		virtual void deserializeFrom(std::queue<std::string> &args);
		// Called either during dragging or when the mouse is released.
		// Needs to update positional data.
		virtual bool onEndAnnotation(QPoint currentPosition) { origin = currentPosition;  return true; };
		virtual QRectF boundingRect() const { return QRect(origin, origin).marginsAdded({ 20, 20, 20, 20 }); }
		// Used for mouse-click events.
		// Returns the handle that has been clicked.
		virtual QPoint *getHandleForPosition(const QPoint &pos)
		{
			if (isHandleAtPosition(origin, pos)) return &origin;
			return nullptr;
		}
		// Static, so that the view can use it to draw special handles.
		static void drawHandleLocation(QPainter *painter, QPoint pos);
	protected:
		bool isHandleAtPosition(const QPoint &handle, const QPoint &pos);
	};

	// A label marks a position.
	struct AnnotationLabel : public Annotation
	{
		using Annotation::Annotation;
		virtual ~AnnotationLabel() = default;

		virtual std::string name() const override { return "label"; }
		virtual void deserializeFrom(std::queue<std::string> &args);
		virtual std::vector<std::string> serializeToVector() const;
		virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) const override;
	};

	// An arrow defines a vector between two positions.
	struct AnnotationArrow : public Annotation
	{
		using Annotation::Annotation;
		virtual ~AnnotationArrow() = default;

		virtual std::string name() const override { return "arrow"; }
		virtual void deserializeFrom(std::queue<std::string> &args);
		virtual std::vector<std::string> serializeToVector() const;
		virtual bool onEndAnnotation(QPoint currentPosition) override { arrowHead = currentPosition; return true; };
		virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) const override;
		virtual QRectF boundingRect() const { return QRect(origin, arrowHead).marginsAdded({ 20, 20, 20, 20 }); }
		virtual QPoint *getHandleForPosition(const QPoint &pos) override;

		QPoint arrowHead;
	};

	// A rectangle defined by two points (top-left and bottom-right)
	struct AnnotationRect : public Annotation
	{
		using Annotation::Annotation;
		virtual ~AnnotationRect() = default;

		virtual std::string name() const override { return "rect"; }
		virtual void deserializeFrom(std::queue<std::string> &args);
		virtual std::vector<std::string> serializeToVector() const;
		virtual bool onEndAnnotation(QPoint currentPosition) override { bottomRight = currentPosition; return true; };
		virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) const override;
		virtual QRectF boundingRect() const { return QRect(origin, bottomRight).marginsAdded({ 20, 20, 20, 20 }); }
		virtual QPoint *getHandleForPosition(const QPoint &pos) override;

		QPoint bottomRight;
	};
	// An ellipse defined by two points (top-left and bottom-right)
	struct AnnotationEllipse : public Annotation
	{
		using Annotation::Annotation;
		virtual ~AnnotationEllipse() = default;

		virtual std::string name() const override { return "rect"; }
		virtual void deserializeFrom(std::queue<std::string> &args);
		virtual std::vector<std::string> serializeToVector() const;
		virtual bool onEndAnnotation(QPoint currentPosition) override { bottomRight = currentPosition; return true; };
		virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) const override;
		virtual QRectF boundingRect() const { return QRect(origin, bottomRight).marginsAdded({ 20, 20, 20, 20 }); }
		virtual QPoint *getHandleForPosition(const QPoint &pos) override;

		QPoint bottomRight;
	};


	void serialize() const;
	void deserialize();
	// Called by the controller to begin a new annotation.
	void startArrow(QPoint origin, size_t currentFrame);
	void startLabel(QPoint origin, size_t currentFrame);
	void startRect(QPoint origin, size_t currentFrame);
	void startEllipse(QPoint origin, size_t currentFrame);
	// Called by the controller on mouse-clicks.
	bool tryStartDragging(QPoint cursor);
	// Called by the controller during mouse-drags to update the current annotation.
	bool updateAnnotation(QPoint cursor);
	// Called on mouse-release.
	bool endAnnotation(QPoint cursor);
	// Removes the currently selected (through tryStartDragging) annotation.
	bool removeSelection();
	// The current frame is required for the view to highlight annotations.
	void setCurrentFrame(size_t currentFrame) { this->currentFrame = currentFrame; }
	size_t getCurrentFrame() const { return currentFrame; }
private:
	// The current frame is required by the view.
	size_t currentFrame{ 0 };
	// filepath of the original source image.
	std::string filepath;
	// The resulting filename will add a suffix to the original source path.
	std::string getFilename() const;
	// Whether the annotations need to be serialized on exit.
	mutable bool dirty{ false };

	std::vector<std::shared_ptr<Annotation>> annotations;
	// Held temporarily during events - not yet 'created'.
	std::shared_ptr<Annotation> currentAnnotation;
	// Valid during drag & drop or after mouse selection.
	struct SelectionData
	{
		// Annotation from the annotations vector above. Defines validity of the whole struct.
		std::weak_ptr<Annotation> annotation;
		QPoint *handle{ nullptr };
		explicit operator bool() const { return !annotation.expired(); }
		// This is likely not thread-safe.
		Annotation *operator->() { return annotation.lock().get(); }
		void reset() { return annotation.reset(); }
	} selection;
	
	friend class AnnotationsView;
};

std::ostream& operator<<(std::ostream& stream, const Annotations::Annotation *annotation);
