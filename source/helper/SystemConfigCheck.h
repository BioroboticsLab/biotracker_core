#pragma once

class QString;

class SystemConfigCheck
{
public:
	SystemConfigCheck(void);
	~SystemConfigCheck(void);

	/**
	 * Check the system supports openGL.
	 * @param: notificationMessage, the notification message,
	 * @return: true, system supports openGL, false otherwise.
	 */
	bool checkOpenGLSupport(QString* notificationMessage);

	/**
	 * Check the system has an initialized config.ini file.
	 * @param: notificationMessage, the notification message,
	 * @return: true, system has config.ini file, false otherwise.
	 */
	bool checkConfigFileExist(QString* notificationMessage);
	

	/**
	 * Check the system configuration on running stable.
	 * @param: notificationMessage, the notification message,
	 * @return: true, if whole system is ready for proper running, false otherwise.
	 */
	bool checkAll(QString* notificationMessage);

	/**
	 * Create a template configuration file.
	 * @param: notificationMessage, the notification message,
	 * @return: true, if creatian is successful, false otherwise.
	 */
	bool createTemplateConfigFile(QString* notificationMessage);
};

