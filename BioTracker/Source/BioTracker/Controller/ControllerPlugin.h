#ifndef CONTROLLERPLUGIN_H
#define CONTROLLERPLUGIN_H

#include "Interfaces/IController/icontroller.h"
#include "Interfaces/IBioTrackerPlugin.h"
#include "QPluginLoader"

class ControllerPlugin : public IController
{
    Q_OBJECT
public:
    ControllerPlugin(QObject *parent = 0, IBioTrackerContext *context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);

    void loadPlugin(QString str);

    // IController interface
protected:
    void createModel() override;
    void createView() override;
    void connectModelController() override;
    void connectController() override;

    void connectPlugin();
    void disconnectPlugin();

private Q_SLOTS:
    void receiveCurrentFrame(std::shared_ptr<cv::Mat> mat);

private:

    IBioTrackerPlugin *m_BioTrackerPlugin;

    QPluginLoader *m_PluginLoader;
};

#endif // CONTROLLERPLUGIN_H
