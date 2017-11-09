#ifndef TRACKERPARAMETER_H
#define TRACKERPARAMETER_H


#include "Interfaces/IModel/IModel.h"
#include "settings/Settings.h"
#include "Model/TrackingAlgorithm/property/ParamNames.h"

class TrackerParameter : public IModel
{
    Q_OBJECT
public:
    TrackerParameter(QObject *parent = 0);

	BioTracker::Core::Settings *getSettings() { return _settings; };

	int getWndSize() { return m_wndSize; };
	void setWndSize(int x) {
        m_wndSize = x;
		Q_EMIT notifyView();
	};

	//std::string getNewSelection() { return m_newSelection; };
	//void setNewSelection(std::string x) {
	//	m_newSelection = x;
	//}
	

	void setAll(
		int wndSize)
	{
        m_wndSize = wndSize;
		_settings->setParam(LUKASKANADE::TRACKERPARAM::WNDSIZE, wndSize);
		Q_EMIT notifyView();
	};



private:
	BioTracker::Core::Settings *_settings;

	int m_wndSize;

};

#endif // TRACKERPARAMETER_H
