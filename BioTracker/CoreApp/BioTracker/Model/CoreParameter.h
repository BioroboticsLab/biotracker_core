#ifndef COREPARAMETER_H
#define COREPARAMETER_H


#include "Interfaces/IModel/IModel.h"
#include "qcolor.h"

class CoreParameter : public IModel
{
	Q_OBJECT
public:
	CoreParameter(QObject *parent = 0);

	void setAll(
		bool viewSwitch,
		QString tracingStyle,
		int tracingHistory,
		int tracingSteps,
		QColor* colorBorder,
		QColor* colorBrush)
	{
		m_viewSwitch = viewSwitch;
		m_tracingStyle = tracingStyle;
		m_tracingHistory = tracingHistory;
		m_tracingSteps = tracingSteps;
		m_colorBorder = colorBorder;
		m_colorBrush = colorBrush;

		Q_EMIT notifyView();
	};
	void setTrackNumber(int number);



public:
	bool m_viewSwitch = true;
	bool m_antialiasingEntities = false;
	bool m_antialiasingFull = false;

	//Tracing
	QString m_tracingStyle = "No tracing";
	int m_tracingHistory = 20;
	int m_tracingSteps = 1;
	QString m_tracingTimeDegradation = "None";
	double m_tracerProportions = 0.5;
	bool m_tracerOrientationLine = false;
	bool m_tracerFrameNumber = false;
	//Track dimensions
	int m_trackWidth = 30;
	int m_trackHeight = 20;
	//Track misc
	bool m_trackOrientationLine = true;
	bool m_trackShowId = false;
	//Track color
	QColor* m_colorBorder = new QColor(Qt::black);
	QColor* m_colorBrush = new QColor(Qt::green);
	//Track count
	int m_trackNumber = 0;
	//Ignore zooming
	bool m_ignoreZoom = false;


};

#endif // COREPARAMETER_H
