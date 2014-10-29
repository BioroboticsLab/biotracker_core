#include "SystemConfigCheck.h"

#include <iostream>

#include <QTextStream>
#include <QGLFormat>
#include <QString>
#include <QFile>

#include "source/settings/Messages.h"
#include "source/settings/ParamNames.h"

SystemConfigCheck::SystemConfigCheck(void)
{
}

SystemConfigCheck::~SystemConfigCheck(void)
{
}

bool SystemConfigCheck::checkOpenGLSupport(QString* notificationMessage)
{
	if (!QGLFormat::hasOpenGL()) {
		notificationMessage->clear();
		notificationMessage->append(QString::fromStdString(MSGS::SYSTEM::NO_OPENGL));
        return false;
    }

	return true;
}

bool SystemConfigCheck::checkConfigFileExist(QString* notificationMessage)
{
	if (!QFile::exists(QString::fromStdString(CONFIGPARAM::CONFIGURATION_FILE))) {
		notificationMessage->clear();
		notificationMessage->append(QString::fromStdString(MSGS::SYSTEM::NO_CONFIGURATION_FILE));        
        return false;
    }

	return true;
}

bool SystemConfigCheck::checkAll(QString* notificationMessage)
{
	bool systemCheckOK = true;
	int checkTime = 2;

	for (int i = 0; i < checkTime; i++)
	{
		if(!(systemCheckOK &= checkConfigFileExist(notificationMessage)))
		{
			// try create the template configuration file
			systemCheckOK = createTemplateConfigFile(notificationMessage);
			continue;
		}

		if(!(systemCheckOK &= checkOpenGLSupport(notificationMessage))) break;

		// if all success the break the loop;
		break;
	}

	return systemCheckOK;
}

bool SystemConfigCheck::createTemplateConfigFile(QString* notificationMessage)
{
	notificationMessage->clear();
	QFile file(QString::fromStdString(CONFIGPARAM::CONFIGURATION_FILE));
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&file);

		out << "[APPLICATIONPARAM]\n";
		out << "APP_VERSION=" << QString::fromStdString(APPLICATIONPARAM::APP_VERSION_NUM) << "\n";
		out << "\n";
		out << "[TRACKERPARAM]\n";
		out << "TRACKING_ENABLED=false\n";
		out << "TRACKING_METHOD=0\n";
		out << "\n";
		out << "[CAPTUREPARAM]\n";
		out << "CAP_VIDEO_FILE=\n";
		out << "CAP_PAUSED_AT_FRAME=\n";
		out << "CAP_SCREENSHOT_PATH=\n";
		out << "\n";
		out << "[GUIPARAM]\n";
		out << "IS_SOURCE_VIDEO=true\n";
		out << "\n";
		out << "[PICTUREPARAM]\n";
		out << "PICTURE_FILE=\n";		

		file.close();
		
		notificationMessage->append(QString::fromStdString(MSGS::SYSTEM::CONFIGURATION_FILE_CREATED));

		return true;
	}

	notificationMessage->append(QString::fromStdString(MSGS::SYSTEM::CONFIGURATION_FILE_CANNOT_BE_CREATED));
	return false;
}