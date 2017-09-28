#ifndef COREPARAMETER_H
#define COREPARAMETER_H


#include "Interfaces/IModel/IModel.h"
//#include "settings/Settings.h"
//#include "Model/TrackingAlgorithm/property/ParamNames.h"

class CoreParameter : public IModel
{
	Q_OBJECT
public:
	CoreParameter(QObject *parent = 0);

	//void setThreshold(int x);

	//int getThreshold();

	//BioTracker::Core::Settings *getSettings() { return _settings; };


	//int getAreaHeight() { return m_areaHeight; };
	//void setAreaHeight(int x) {
	//	m_areaHeight = x;
	//	_settings->setParam(FISHTANKPARAM::FISHTANK_AREA_HEIGHT, x);
	//	Q_EMIT notifyView();
	//};

	//std::string getNewSelection() { return m_newSelection; };

	//void setNewSelection(std::string x) 
	//{
	//	m_newSelection = x;
	//}	

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
	//BioTracker::Core::Settings *_settings;

	bool m_viewSwitch;
	QString m_tracingStyle;
	int m_tracingHistory;
	int m_tracingSteps;
	QColor* m_colorBorder;
	QColor* m_colorBrush;


	//std::string m_newSelection;
};

#endif // COREPARAMETER_H
