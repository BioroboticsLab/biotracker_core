#pragma once

#include <QString>
#include <memory>
#include <condition_variable>
#include <atomic>
#include <QRect>
#include <QPainter>
#include <QPoint>
#include <zmq.h>
#include <opencv2/opencv.hpp>
#include <QProcess>
#include "zmq/ZmqInfoFile.h"

namespace BioTracker {
namespace Core {
namespace Zmq {

/**
 * @brief The ZmqProcessHandler class
 */
class ZmqProcessHandler : public QObject {
  public:
    Q_OBJECT
  public:
    ZmqProcessHandler(QObject *parent, const ZmqInfoFile info, void *socket);
    ~ZmqProcessHandler();

    void stop();

    void start();

  Q_SIGNALS:
    void onError(std::string errorMessage);

  private Q_SLOTS:
    void processBadError(QProcess::ProcessError error);
    void processError();

  private:
    void *m_socket;
    std::mutex m_zmqMutex;
    std::unique_ptr<QProcess> m_zmqClient;

};

}
}
}
