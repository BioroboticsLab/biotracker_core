#pragma once

#include <QString>
#include <QRect>
#include <QProcess>
#include <QPainter>
#include <QPoint>
#include <zmq.h>
#include <opencv2/opencv.hpp>
#include "zmq/ZmqMessageParser.h"
#include "zmq/ZmqInfoFile.h"

namespace BioTracker {
namespace Core {
namespace Zmq {

const int ZMQ_TIMEOUT_IN_MS = 10000; // timeout for the zmq tracker during execution
const int ZMQ_SMALL_TIMEOUT_IN_MS = 1000; // timeout for the startup of the zmq tracker

class ZmqClientProcess: public QObject {
    Q_OBJECT
  public:
    ZmqClientProcess(ZmqInfoFile file, void *socket, std::string url="tcp://127.0.0.1:5556",
                     const int ts=ZMQ_TIMEOUT_IN_MS);
    ~ZmqClientProcess() {
        shutdown();
    }

    /**
     * @brief shutdown
     * terminates the Process beyond repair
     */
    void shutdown();

    void send(GenericSendMessage &message, EventHandler &handler);

  private Q_SLOTS:
    void processBadError(QProcess::ProcessError error);
    void processError();

  private:
    void track(SendTrackMessage &message, EventHandler &handler);

    void paint(SendPaintMessage &message, EventHandler &handler);

    void paintOverlay(SendPaintOverlayMessage &message, EventHandler &handler);

    void requestTools(SendRequestWidgetsMessage &message, EventHandler &handler);

    void buttonClicked(SendButtonClickMessage &m, EventHandler &handler);

    void sendValue(SendValueChangedMessage &message, EventHandler &handler);

    void sendMessage(GenericSendMessage &message);
    void listenToEvents(EventHandler &handler);
    void requestResults(GenericReceiveMessage &message);

    std::unique_ptr<QProcess> m_zmqClient;
    std::mutex m_zmqMutex;

};

}
}
}
