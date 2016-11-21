#include "PluginLoader.h"

PluginLoader::PluginLoader(QObject *parent = Q_NULLPTR) :
    QPluginLoader(parent)
{
    m_PluginLoader = new QPluginLoader(this);
}

bool PluginLoader::loadPlugin(QString str)
{
    //m_PluginLoader->load();
    return false;
}
