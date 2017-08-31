#include "PluginLoader.h"
#include "QDebug"
#include <iostream>

PluginLoader::PluginLoader(QObject* parent) :
    IModel(parent) {

    m_isPluginLoaded = false;
    m_PluginLoader = new QPluginLoader(this);

    m_PluginListModel = new QStringListModel();
    m_PluginListModel->setStringList(m_PluginList);
}

QString PluginLoader::loadPluginFromFilename(QString filename) {
    bool retval = false;
	QString name = "";

    if(m_PluginLoader->isLoaded()) {

        m_PluginLoader->unload();
    }

    bool isLib = QLibrary::isLibrary(filename);

    if( isLib ) {

        m_PluginLoader->setFileName(filename);

        name = readMetaDataFromPlugin();
		m_PluginHandlesList.insert(std::pair<QString, QPluginLoader*>(name, m_PluginLoader));

		qDebug() << m_PluginLoader->fileName();

		m_PluginLoader = new QPluginLoader(this);

        retval = true;
    } else {
        retval = false;
    }


    m_isPluginLoaded = retval;

	return name;
}

IBioTrackerPlugin* PluginLoader::getPluginInstance(QString name) {
	std::map<QString, QPluginLoader*>::iterator pos = m_PluginHandlesList.find(name);
	if (pos == m_PluginHandlesList.end()) {
		//handle the error
		std::cout << "Could not get plugin instance " << name.toStdString() << std::endl;
		return 0;
	}
	else {
		QPluginLoader* pl = pos->second;
		return qobject_cast<IBioTrackerPlugin*>(pl->instance());//(m_PluginLoader->instance());
	}
}

QStringListModel* PluginLoader::getPluginMetaData() {
    return m_PluginListModel;
}

bool PluginLoader::getIsPluginLoaded() {
    return m_isPluginLoaded;
}

QString PluginLoader::readMetaDataFromPlugin() {
    QJsonValue pluginMeda(m_PluginLoader->metaData().value("MetaData"));
    QJsonObject metaObj = pluginMeda.toObject();
    QString mstring = metaObj.value("name").toString();

    m_PluginList.append(mstring);
    m_PluginListModel->setStringList(m_PluginList);
	return mstring;
}
