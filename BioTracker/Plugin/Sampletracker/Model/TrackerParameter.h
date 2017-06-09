#ifndef TRACKERPARAMETER_H
#define TRACKERPARAMETER_H


#include "Interfaces/IModel/IModel.h"
#include "settings/Settings.h"

class TrackerParameter : public IModel
{
    Q_OBJECT
public:
    TrackerParameter(QObject *parent = 0);

    void setThreshold(int x);

    int getThreshold(); 
	
	void TrackerParameter::setAll(int a,int b, int c, int d, int e, int f)
	{
		_lowH = a;
		_highH = b;
		_lowS = c;
		_highS = d;
		_lowV = e;
		_highV = f;

		Q_EMIT notifyView();
	}


	// values for filtering a color (HSV format)
	int _lowH;
	int _highH;
	int _lowS;
	int _highS;
	int _lowV;
	int _highV;

private:
	BioTracker::Core::Settings *_settings;

	int m_Threshold;

};

#endif // TRACKERPARAMETER_H
