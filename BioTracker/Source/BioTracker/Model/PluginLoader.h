#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include "QPluginLoader"
#include "QPointer"

class PluginLoader : public QPluginLoader
{
public:
    PluginLoader(QObject *parent);

    bool loadPlugin(QString str);

private:
    QPointer<QPluginLoader> m_PluginLoader;
};

#endif // PLUGINLOADER_H
