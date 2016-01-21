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
#include "ZmqInfoFile.h"
#include "ZmqClientProcess.h"
#include "biotracker/util/singleton.h"

namespace BioTracker {
namespace Core {
namespace Zmq {

/**
 * @brief The ZmqProcessHandler class
 */
class ZmqProcessHandler : public QObject, public Util::Singleton<ZmqProcessHandler> {
  public:
    Q_OBJECT
  public:
    ZmqProcessHandler();
    ~ZmqProcessHandler();

    std::shared_ptr<ZmqClientProcess> startProcess(ZmqInfoFile &info);

  private:
    void *m_context;
    std::mutex m_zmqMutex;
    std::shared_ptr<ZmqClientProcess> m_currentProcess;
};

}
}
}
