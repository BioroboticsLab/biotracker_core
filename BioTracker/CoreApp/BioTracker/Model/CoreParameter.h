#ifndef COREPARAMETER_H
#define COREPARAMETER_H


#include "Interfaces/IModel/IModel.h"

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



public:
	bool m_viewSwitch;
	QString m_tracingStyle;
	int m_tracingHistory;
	int m_tracingSteps;
	QColor* m_colorBorder;
	QColor* m_colorBrush;
};

#endif // COREPARAMETER_H
