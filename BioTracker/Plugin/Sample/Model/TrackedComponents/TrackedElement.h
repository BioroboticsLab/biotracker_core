#ifndef TRACKEDELEMENT_H
#define TRACKEDELEMENT_H

#include "QGraphicsItem"
#include "Interfaces/IModel/IModelTrackedComponent.h"
#include "QString"

namespace sampleTracker {
	/**
	* This class is an example of how a TrackedComponent could be defined.
	* This class inherits from the IModelTrackedComponent class and is therefor part of the Composite Pattern.
	* This class represents the Leaf class in the Composite Pattern.
	* Objects of this class have a QObject as parent.
	*/
	class TrackedElement : public IModelTrackedComponent
	{
		Q_OBJECT

	public:
		TrackedElement(QObject *parent = 0, QString name = "n.a.", int id = 0);

		QString getName();

		void  setX(float val) {};
		void  setY(float val) {};
		void  setZ(float z) {};
		void  setW(float w) {};
		void  setH(float h) {};
		void  setRad(float r) {};
		void  setDeg(float d) {};
		void  setId(int val) { _id = val; };
		void  setTime(std::chrono::steady_clock::time_point t) { _time = t; };
		void  setValid(bool v) { _valid = v; };

		float getX() { return 0; };
		float getY() { return 0; };
		float getZ() { return 0; };
		float getW() { return 0; };
		float getH() { return 0; };
		float getRad() { return 0; };
		float getDeg() { return 0; };
		int   getId() { return _id; };
		std::chrono::steady_clock::time_point  getTime() { return _time; };
		bool  getValid() { return _valid; };

		bool hasX() { return false; };
		bool hasY() { return false; };
		bool hasZ() { return false; };
		bool hasW() { return false; };
		bool hasH() { return false; };
		bool hasRad() { return false; };
		bool hasDeg() { return false; };
		bool hasTime() { return true; };

		void pressed();
		void notPressed();
		bool getPressedStatus();

		// ITrackedComponent interface
	public:
		void operate();

	private:
		QString _name;
		int _id;
		bool _valid;

		bool _pressed;
		std::chrono::steady_clock::time_point _time;
	};
}
#endif // TRACKEDELEMENT_H
