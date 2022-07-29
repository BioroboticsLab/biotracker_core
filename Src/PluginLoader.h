
#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include "QPluginLoader"
#include "QStringListModel"
#include "QPointer"
#include <map>
#include <memory>

/**
 * \class PluginLoader
 * This class loads Qt-Plugins, the related metadata and files the plugin
 * depends on. \author Hauke Mönck, Gregor Barth
 */

class PluginLoader : QObject
{
	Q_OBJECT
public:
	explicit PluginLoader(QObject* parent = 0);

	~PluginLoader();

	/**
	 * Loads a file as a QT plugin and reads it's name from the metadata.
	 * It will then be added to the stringlist and is selectable via
	 * "loadPluginFromName". This function does not actually set the plugin
	 * instance.
	 */
	int addToPluginList(QString filename, QString suffix);

	/**
	 * Returns a QStrinListModel with the names of all seen Plugins.
	 */
	QStringListModel* getPluginList();

	/**
	 * Loads a BioTracker Plugin from a file path. It returns true if the Plugin
	 * could be loaded, otherwise false. The loaded plugin acts as singleton!
	 */
	bool loadPluginFromFilename(QString const& filename);

	/**
	 * Loads a plugin from it's name (given in the metadata) instead of the
	 * filename. Only works if it is already known to the pluginloader, i.e. was
	 * loaded manually or automatically before or or has been added to the
	 * pluginlist via "addToPluginList"
	 */
	bool loadPluginFromName(QString name);

	QJsonObject getPluginMetaData() const;

	static std::vector<std::string> queryRegistryBehaviors(std::string path);

	static std::vector<std::string> searchDirectoriesForPlugins(
	   std::vector<std::string> list,
	   std::string              suffix);

	// return oldPath
	static const char* addDllPath(std::string file);

	static void delDllPath(const char* oldPath);

	// Is a plugin loaded or not?
	bool getIsPluginLoaded();

	// Gets the name of the currently loaded plugin
	QString getCurrentPluginName();

	/**
	 * Returns the Instance of the BioTracker Plugin.
	 */
	QObject* getPluginInstance();

	/**
	 * Returns a map containing the mapping "plugin name -> filename"
	 */
	std::map<QString, QString> const& getPluginMap() const;

private:
	void addPluginnameToLists(QString name, QString filename);

	// a map containing the mapping "plugin name -> filename"
	std::map<QString, QString> m_PluginMap;

	/**
	 * Gets the metadata from the currently loaded plugin.
	 * Currently this is nothing but the advertised name of the plugin
	 */
	void readMetaDataFromPlugin();

	// The QT object to actually load the plugins
	QPluginLoader* m_PluginLoader;

	std::shared_ptr<QJsonObject> m_MetaData;

	// nomen est omen
	bool m_isPluginLoaded;

	// List of all available plugins
	QStringList m_PluginList;

	// Entire ListModel of the metadata (actually containing all metadata, not
	// only name)
	QStringListModel* m_PluginListModel;
};

#endif // PLUGINLOADER_H
