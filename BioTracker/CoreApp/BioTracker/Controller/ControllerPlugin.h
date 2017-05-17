#ifndef CONTROLLERPLUGIN_H
#define CONTROLLERPLUGIN_H

#include "Interfaces/IController/IController.h"
#include "Interfaces/IBioTrackerPlugin.h"


class ControllerPlugin : public IController {
    Q_OBJECT
  public:
    ControllerPlugin(QObject* parent = 0, IBioTrackerContext* context = 0, ENUMS::CONTROLLERTYPE ctr = ENUMS::CONTROLLERTYPE::NO_CTR);

    void loadPluginFromFileName(QString str);
    void sendCurrentFrameToPlugin(std::shared_ptr<cv::Mat> mat, uint number);

    // IController interface
  protected:
    void createModel() override;
    void createView() override;
    void connectModelToController() override;
    void connectControllerToController() override;

    void createPlugin();

    void connectPlugin();
    void disconnectPlugin();

  private Q_SLOTS:
    void receiveTrackingDone();

  private:
    IBioTrackerPlugin* m_BioTrackerPlugin;



};

#endif // CONTROLLERPLUGIN_H
