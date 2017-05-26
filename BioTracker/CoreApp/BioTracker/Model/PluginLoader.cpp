#include "PluginLoader.h"
#include "QDebug"

PluginLoader::PluginLoader(QObject* parent) :
    IModel(parent) {

    m_isPluginLoaded = false;
    m_PluginLoader = new QPluginLoader(this);

    m_PluginListModel = new QStringListModel();
    m_PluginListModel->setStringList(m_PluginList);
}

bool PluginLoader::loadPluginFromFilename(QString filename) {
    bool retval = false;

    if(m_PluginLoader->isLoaded()) {

        m_PluginLoader->unload();
    }

    bool isLib = QLibrary::isLibrary(filename);

    if( isLib ) {

        m_PluginLoader->setFileName(filename);

        retval = m_PluginLoader->load();

        qDebug() << m_PluginLoader->errorString();

        readMetaDataFromPlugin();


    } else {
        retval = false;
    }

    qDebug() << m_PluginLoader->fileName();

    m_isPluginLoaded = retval;

    return retval;
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

void PluginLoader::readMetaDataFromPlugin() {
    QJsonValue pluginMeda(m_PluginLoader->metaData().value("MetaData"));
    QJsonObject metaObj = pluginMeda.toObject();
    QString mstring = metaObj.value("name").toString();

    m_PluginList.append(mstring);
    m_PluginListModel->setStringList(m_PluginList);
}
