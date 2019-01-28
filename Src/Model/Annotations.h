/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg and BioTracker in version 2.
  **
  **
  ****************************************************************************/


#pragma once

#include "Interfaces/IModel/IModel.h"

#include <QList>
#include "QString"
#include <QMap>
#include <QPoint>
#include <QPainter>
#include <QVariant>
#include <QGraphicsItem>

#include <iostream>
#include <vector>
#include <memory>
#include <queue>

class IModelTrackedComponent;

/**
	Model to handle annotations, including serialization.
	Annotations will be drawn on top of the video. Their position is defined in image coordinates.
*/
class Annotations : public IModel {
	Q_OBJECT
public:
	Annotations(std::string filepath = "") : filepath(filepath) { deserialize(); }
	virtual ~Annotations();

	/*
		A 2d point that can be linked to a tracked component ID.
	 */
	struct TrackedPoint {
		TrackedPoint (const QPoint &pos) : position(pos) {}
		TrackedPoint (int x, int y) : TrackedPoint(QPoint(x, y)) {}
		TrackedPoint (const QPoint &pos, int trackID) : TrackedPoint(pos)
		{
			this->trackID = trackID;
			isLinkedToTrack = true;
		}
		TrackedPoint() {}
		QPoint position {0, 0};
		int trackID {0};
		bool isLinkedToTrack {0};

		// Retrieve the current position.
		QPoint getPoint() const { return position; }
		const QPoint &operator *() const { return position; }
		int x() const { return position.x(); }
		int y() const { return position.y(); }
		// Update the current position to the corresponding track position IIF isLinkedToTrack.
		void update(int currentFrameID, const QList<IModelTrackedComponent*> &trackedComponents);
	};
	/*
		Base annotation class.
		Children must at least support serialization and drawing.
	*/
	struct Annotation
	{
		Annotation(QPoint origin = { 0,0 }, size_t startFrame = 0) : origin(origin), startFrame(startFrame) {}
		virtual ~Annotation() = default;
		// Position in pixels.
		TrackedPoint origin{ 0, 0 };
		size_t startFrame{ 0 }, endFrame{ 0 };
		// Possible text for each annotation
		QString text{ "" };
		/// Name that identifies this type of annotation and is used for serialization.
		virtual std::string name() const = 0;
		virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) const = 0;
		// Saving requires writing/reading properties to/from maps.
		virtual QMap<QString, QVariant> serializeToMap() const;
		virtual void deserializeFrom(const QMap<QString, QVariant> &map);
		// Set text. Called when right clicking.
		virtual void setText(QString newText) { text = newText; }
		// Get text. Called when right clicking.
		virtual QString getText() { return text; }
		// Called either during dragging or when the mouse is released.
		// Needs to update positional data.
		virtual bool onEndAnnotation(TrackedPoint currentPosition) { origin = currentPosition;  return true; }
		virtual QRectF boundingRect() const { return QRect(*origin, *origin).marginsAdded({ 20, 20, 20, 20 }); }
		// Used for mouse-click events.
		// Returns the handle that has been clicked.
		virtual TrackedPoint *getHandleForPosition(const QPoint &pos)
		{
			if (isHandleAtPosition(origin, pos)) return &origin;
			return nullptr;
		}
		// Static, so that the view can use it to draw special handles.
		static void drawHandleLocation(QPainter *painter, QPoint pos, QString text);
		static void drawHandleLocation(QPainter *painter, const TrackedPoint &pos, QString text)
		{
			drawHandleLocation(painter, *pos, text);
		}
		virtual void updateTrackedPositions(int currentFrameID, const QList<IModelTrackedComponent*> &trackedComponents)
		{
			origin.update(currentFrameID, trackedComponents);
		}
	protected:
		bool isHandleAtPosition(const QPoint &handle, const QPoint &pos);
		bool isHandleAtPosition(const TrackedPoint &handle, const QPoint &pos)
		{
			return isHandleAtPosition(*handle, pos);
		}
	};

	/// A label marks a position.
	struct AnnotationLabel : public Annotation
	{
		using Annotation::Annotation;
		virtual ~AnnotationLabel() = default;

		virtual std::string name() const override { return "label"; }
		virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) const override;
	};

	/// An arrow defines a vector between two positions.
	struct AnnotationArrow : public Annotation
	{
		using Annotation::Annotation;
		virtual ~AnnotationArrow() = default;

		virtual std::string name() const override { return "arrow"; }
		virtual QMap<QString, QVariant> serializeToMap() const override;
		virtual void deserializeFrom(const QMap<QString, QVariant> &map) override;
		virtual bool onEndAnnotation(TrackedPoint currentPosition) override { arrowHead = currentPosition; return true; }
		virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) const override;
		virtual QRectF boundingRect() const { return QRect(*origin, *arrowHead).marginsAdded({ 20, 20, 20, 20 }); }
		virtual TrackedPoint *getHandleForPosition(const QPoint &pos) override;
		virtual void updateTrackedPositions(int currentFrameID, const QList<IModelTrackedComponent*> &trackedComponents) override;

		TrackedPoint arrowHead;
	};

	// A rectangle defined by two points (top-left and bottom-right)
	struct AnnotationRect : public AnnotationArrow
	{
		using AnnotationArrow::AnnotationArrow;
		virtual ~AnnotationRect() = default;

		virtual std::string name() const override { return "rect"; }
		virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) const override;
	};
	// An ellipse defined by two points (top-left and bottom-right)
	struct AnnotationEllipse : public AnnotationArrow
	{
		using AnnotationArrow::AnnotationArrow;
		virtual ~AnnotationEllipse() = default;

		virtual std::string name() const override { return "ellipse"; }
		virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) const override;
	};

	void serialize() const;
	void deserialize();
	/// Called by the controller to begin a new annotation.
	void startArrow(QPoint origin, size_t currentFrame);
	void startLabel(QPoint origin, size_t currentFrame);
	void startRect(QPoint origin, size_t currentFrame);
	void startEllipse(QPoint origin, size_t currentFrame);
	/// Called by the controller on left mouse-clicks.
	bool tryStartDragging(QPoint cursor);
	/// Called by the controller on left mouse-clicks.
	bool trySetText(QPoint cursor);
	// Called by the controller during mouse-drags to update the current annotation.
	bool updateAnnotation(TrackedPoint cursor);
	// Called on mouse-release.
	bool endAnnotation(TrackedPoint cursor);
	// Removes the currently selected (through tryStartDragging) annotation.
	bool removeSelection();
	// Sets the start frame of the current selection to the current frame.
	bool updateSelectionStartFrame();
	// Sets the end frame of the current selection to the current frame.
	bool updateSelectionEndFrame();
	// The current frame is required for the view to highlight annotations.
	void setCurrentFrame(size_t currentFrame) { this->currentFrame = currentFrame; }
	size_t getCurrentFrame() const { return currentFrame; }
	// Called when trajectories or displayed frames have changed.
	void updateTrackedAnnotations(const QList<IModelTrackedComponent*> &trackedComponents);
private:
	size_t currentFrame{ 0 };							/**< The current frame is required by the view. */

	std::string filepath;								/**< filepath of the original source image.  */

	std::string getFilename() const;					/**< The resulting filename will add a suffix to the original source path. */

	mutable bool dirty{ false };						/**< Whether the annotations need to be serialized on exit.  */

	std::vector<std::shared_ptr<Annotation>> annotations;/**< vector of all annotations  */

	std::shared_ptr<Annotation> currentAnnotation;		/**< Held temporarily during events - not yet 'created'.  */

	/// Valid during drag & drop or after mouse selection.
	struct SelectionData
	{
		// Annotation from the annotations vector above. Defines validity of the whole struct.
		std::weak_ptr<Annotation> annotation;
		TrackedPoint *handle{ nullptr };
		explicit operator bool() const { return !annotation.expired(); }
		// This is likely not thread-safe.
		Annotation *operator->() { return annotation.lock().get(); }
		void reset() { return annotation.reset(); }
	} selection;

	friend class AnnotationsView;
};

