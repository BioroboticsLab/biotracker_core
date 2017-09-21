#include "PluginLoader.h"
#include "QDebug"
#include <iostream>

PluginLoader::PluginLoader(QObject* parent) :
    IModel(parent) {

    m_isPluginLoaded = false;
	m_currentPluginName = "";
    m_PluginLoader = new QPluginLoader(this);

    m_PluginListModel = new QStringListModel();
    m_PluginListModel->setStringList(m_PluginList);
}

void PluginLoader::addToPluginList(QString filename) {

	bool isLib = QLibrary::isLibrary(filename);

	if (isLib) {

		QPluginLoader loader;
		loader.setFileName(filename);
		QJsonValue pluginMeda(loader.metaData().value("MetaData"));
		QJsonObject metaObj = pluginMeda.toObject();
		QString mstring = metaObj.value("name").toString();
		if (!m_PluginList.contains(mstring))
			m_PluginList.append(mstring);
		m_PluginListModel->setStringList(m_PluginList);
		m_PluginMap.insert(std::pair<QString, QString>(mstring, filename));
	}
	else {
		std::cout << "Error reading plugin: " << filename.toStdString() << std::endl;
	}
}

bool PluginLoader::loadPluginFromFilename(QString filename) {
	bool retval = false;

	if (m_PluginLoader->isLoaded()) {
		m_PluginLoader->unload();
	}

	bool isLib = QLibrary::isLibrary(filename);

	if (isLib) {

		m_PluginLoader->setFileName(filename);
		QString name = readMetaDataFromPlugin();
		m_currentPluginName = name;
		m_PluginMap.insert(std::pair<QString, QString>(name, filename));
		retval = true;
	}
	else {
		retval = false;
	}

	qDebug() << m_PluginLoader->fileName();

	m_isPluginLoaded = retval;

	return retval;
}

bool PluginLoader::loadPluginFromName(QString name) {
	QString filename = m_PluginMap.find(name)->second;
	return loadPluginFromFilename(filename);
}

IBioTrackerPlugin* PluginLoader::getPluginInstance() {
	return qobject_cast<IBioTrackerPlugin*>(m_PluginLoader->instance());
}

QStringListModel* PluginLoader::getPluginMetaData() {
	return m_PluginListModel;
}

bool PluginLoader::getIsPluginLoaded() {
	return m_isPluginLoaded;
}

QString PluginLoader::getCurrentPluginName() {
	return m_currentPluginName;
}

const std::map<QString, QString> &PluginLoader::getPluginMap() const
{
    return m_PluginMap;
}

QString PluginLoader::readMetaDataFromPlugin() {
	QJsonValue pluginMeda(m_PluginLoader->metaData().value("MetaData"));
	QJsonObject metaObj = pluginMeda.toObject();
	QString mstring = metaObj.value("name").toString();

	if (!m_PluginList.contains(mstring))
		m_PluginList.append(mstring);
	m_PluginListModel->setStringList(m_PluginList);
	return mstring;
}

