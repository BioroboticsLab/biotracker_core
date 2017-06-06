#pragma once

#include <iostream>
#include <vector>
#include <regex>

#include <opencv2/opencv.hpp>

#include <QtCore/QSettings>
#include <QtCore/QString>

#include "Model/TrackingAlgorithm/property/ParamNames.h"
#include "Model/TrackingAlgorithm/property/Param.h"

class SystemProperty
{
public:

	static SystemProperty& instance()
	{
		static SystemProperty _instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return _instance;
	}

	/**
	* The standard destructor.
	*/
	~SystemProperty() {}

	static const int DISPLAYWINDOWWIDTH = 640; //TODO IMPORTANT EVIL!
	static const int DISPLAYWINDOWHEIGHT = 480;

private:

	/**
	* The standard constructor.
	*/
	SystemProperty(void);

	SystemProperty(const SystemProperty&); /* verhindert, dass eine weitere Instanz via
										 Kopier-Konstruktor erstellt werden kann */

	SystemProperty & operator = (const SystemProperty &); //Verhindert weitere Instanz durch Kopie

public:

	
private:
	//std::vector<Param> _params;
};
