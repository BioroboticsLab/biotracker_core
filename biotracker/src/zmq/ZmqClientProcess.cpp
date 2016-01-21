#include "biotracker/zmq/ZmqClientProcess.h"

#include <opencv/cv.hpp>
#include <QProcess>
#include <QString>

#include "biotracker/zmq/ZmqHelper.h"

namespace BioTracker {
namespace Core {
namespace Zmq {

bool isFree = true;
void *SOCKET; // TODO make this a member variable. To do so, we need to provide the socket
// to the callback functions (to receive string and receive mat) somehow

// =========================================
// PUBLIC
// =========================================

ZmqClientProcess::ZmqClientProcess(ZmqInfoFile info, void *socket, std::string url, const int ts) {
    if (isFree) { // there can be only ONE ZmqClientProcess at the same time!
        SOCKET = socket;
        isFree = false;
    } else {
        throw std::invalid_argument("Socket is already in use and must be shutdown`d before using it again");
    }

    zmq_setsockopt(SOCKET, ZMQ_RCVTIMEO, &ts, sizeof(int));

    int rc = zmq_bind(SOCKET, url.c_str());
    if (rc != 0) {
        int rno = zmq_errno();
        QString err_msg(zmq_strerror(rno));
        throw std::invalid_argument(err_msg.toStdString());
    }

    m_zmqClient = std::make_unique<QProcess>(this);
    m_zmqClient->setProcessChannelMode(QProcess::ForwardedChannels);
    QString command = info.m_program + " " + info.m_arguments.first();

    QObject::connect(m_zmqClient.get(), &QProcess::readyReadStandardError,
                     this, &ZmqClientProcess::processError);

    QObject::connect(m_zmqClient.get(),
                     static_cast<void(QProcess::*)(QProcess::ProcessError)>(&QProcess::error),
                     this, &ZmqClientProcess::processBadError);

    m_zmqClient->start(command);
    m_zmqClient->waitForStarted();
}


void ZmqClientProcess::send(GenericSendMessage &message, EventHandler &handler) {
    m_zmqMutex.lock();
    switch (message.type) {
    case None: {
        throw std::invalid_argument("Message type must not be 'None'");
        break;
    }
    case Track: {
        auto &m = static_cast<SendTrackMessage &>(message);
        track(m, handler);
        break;
    }
    case Paint: {
        auto &m = static_cast<SendPaintMessage &>(message);
        paint(m, handler);
        break;
    }
    case PaintOverlay: {
        auto &m = static_cast<SendPaintOverlayMessage &>(message);
        paintOverlay(m, handler);
        break;
    }
    case RequestTools: {
        auto &m = static_cast<SendRequestWidgetsMessage &>(message);
        requestTools(m, handler);
        break;
    }
    case ButtonClick: {
        auto &m = static_cast<SendButtonClickMessage &>(message);
        buttonClicked(m, handler);
        break;
    }
    case ValueChanged: {
        auto &m = static_cast<SendValueChangedMessage &>(message);
        sendValue(m, handler);
        break;
    }
    }
    m_zmqMutex.unlock();
}

void ZmqClientProcess::shutdown() {
    isFree = true;
    m_zmqMutex.lock();
    send_string(SOCKET, TYPE_SHUTDOWN, 0);
    EventHandler temp(this);
    listenToEvents(temp);
    m_zmqClient->kill();
    m_zmqClient->waitForFinished(1000);
    zmq_disconnect(SOCKET, "172.0.0.1:5556");
    zmq_close(SOCKET);
    m_zmqMutex.unlock();
}

// =========================================
// PRIVATE
// =========================================

void ZmqClientProcess::track(SendTrackMessage &message, EventHandler &handler) {
    sendMessage(message);
    listenToEvents(handler);
}

void ZmqClientProcess::paint(SendPaintMessage &message, EventHandler &handler) {
    ReceivePaintMessage result(message);
    sendMessage(message);
    listenToEvents(handler);
    requestResults(result);
}

void ZmqClientProcess::paintOverlay(SendPaintOverlayMessage &message, EventHandler &handler) {
    ReceiveQPainterMessage result(message);
    sendMessage(message);
    listenToEvents(handler);
    requestResults(result);
}

void ZmqClientProcess::requestTools(SendRequestWidgetsMessage &message, EventHandler &handler) {
    ReceiveToolsWidgetMessage result(message, handler);
    sendMessage(message);
    listenToEvents(handler);
    requestResults(result);
}

void ZmqClientProcess::buttonClicked(SendButtonClickMessage &message, EventHandler &handler) {
    sendMessage(message);
    listenToEvents(handler);
}

void ZmqClientProcess::sendValue(SendValueChangedMessage &message, EventHandler &handler) {
    sendMessage(message);
    listenToEvents(handler);
}

// =========================================
// GENERIC FUNCTIONS
// =========================================

/**
 * Send all the parts of the message, then return
 * @param message
 */
void ZmqClientProcess::sendMessage(GenericSendMessage &message) {
    const std::vector<ZmqMessage> messageParts = message.get();
    const int size = messageParts.size();
    for (int i = 0; i < size; i++) {
        int flag = 0;
        if (i < (size - 1)) {
            flag = ZMQ_SNDMORE;
        }
        ZmqMessage m = messageParts[i];

        if (m.isText) {
            send_string(SOCKET, m.text, flag);
        } else { // send mat DATA
            size_t sizeInBytes = m.mat.total() * m.mat.elemSize();
            zmq_send(SOCKET, m.mat.data, sizeInBytes, flag);
        }
    }
}

QString recvThisString() {
    return recv_string(SOCKET);
}

void recvThatMat(cv::Mat &m) {
    recv_mat(SOCKET, m);
}

/**
 * Receive all events, when the END-Event is send, this function returns
 * @param handler
 */
void ZmqClientProcess::listenToEvents(EventHandler &handler) {
    recvString func = &recvThisString;
    handler.receive(func);
}

/**
 * Receive the message that is specified by the ReceiverMessage
 * @param message
 */
void ZmqClientProcess::requestResults(GenericReceiveMessage &message) {
    recvString func = &recvThisString;
    recvMat funcMat = &recvThatMat;
    message.receive(func, funcMat);
}

// ===========================================
// ===========================================

void BioTracker::Core::Zmq::ZmqClientProcess::processBadError(QProcess::ProcessError error) {
    // TODO implement error handling
}

void BioTracker::Core::Zmq::ZmqClientProcess::processError() {
    // TODO implement error handling
}

}
}
}
