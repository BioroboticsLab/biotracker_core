/****************************************************************************
  **
  ** This file is part of the BioTracker Framework
  ** by Andreas Jörg
  **
  ****************************************************************************/

#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include "Interfaces/IModel/IModel.h"
#include "QPluginLoader"
#include "QStringListModel"
#include "Interfaces/IBioTrackerPlugin.h"
#include "QPointer"

/**
 * The PluginLoader class is a IModel class. It is responsible for managing BioTracker Plugins. It can load them and it sends a list of all loaded Plugin names to the a selection box in the MainWindow.
 */
class PluginLoader : public IModel {
    Q_OBJECT
  public:
	  explicit PluginLoader(QObject* parent = 0);

	  void addToPluginList(QString p);

	  /**
	  * Loads a BioTracker Plugin from a filpaht. It returns true if the Plugin could be loaded, otherwise false.
	  */
	  bool loadPluginFromFilename(QString filename);

	  bool loadPluginFromName(QString name);

	  /**
	  * Returns the Instance of the BioTracker Plugin.
	  */
	  IBioTrackerPlugin* getPluginInstance();

	  /**
	  * Returns a QStrinListModel with the names of all loaded Plugins.
	  */
	  QStringListModel* getPluginMetaData();

	  bool getIsPluginLoaded();

	  std::map<QString, QString> m_PluginMap;

private:

	bool m_isPluginLoaded;

	QString readMetaDataFromPlugin();

	QPluginLoader* m_PluginLoader;

	QStringList m_PluginList;
	QStringListModel* m_PluginListModel;


};

#endif // PLUGINLOADER_H
