#include "PluginLoader.h"
#include "QDebug"

PluginLoader::PluginLoader(QObject *parent) :
    IModel(parent)
{
    m_PluginLoader = new QPluginLoader(this);
}

bool PluginLoader::loadPluginFromFilename(QString filename)
{
    bool retval = false;

    if(m_PluginLoader->isLoaded()) {

        m_PluginLoader->unload();
    }

    bool isLib = QLibrary::isLibrary(filename);

    if( isLib ) {

            m_PluginLoader->setFileName(filename);
            retval = true;
    }
    else {
            retval = false;
    }

    qDebug() << m_PluginLoader->fileName();

    return retval;
}

IBioTrackerPlugin *PluginLoader::getPluginInstance()
{
    return qobject_cast<IBioTrackerPlugin *>(m_PluginLoader->instance());
}
