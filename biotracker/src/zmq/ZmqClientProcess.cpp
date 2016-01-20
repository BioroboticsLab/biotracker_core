#include "zmq/ZmqClientProcess.h"

#include <opencv/cv.hpp>

#include "zmq/ZmqHelper.h"

namespace BioTracker {
namespace Core {
namespace Zmq {

bool isFree = true;
void *SOCKET;

// =========================================
// PUBLIC
// =========================================

BioTracker::Core::Zmq::ZmqClientProcess::ZmqClientProcess(ZmqInfoFile file, void *socket, const int ts):
    m_timeout(ts),
    m_socket(socket) {
    if (isFree) { // there can be only ONE ZmqClientProcess at the same time!
        SOCKET = socket;
        isFree = false;
    } else {
        throw std::invalid_argument("Socket is already in use and must be shutdown`d before using it again");
    }
}


void BioTracker::Core::Zmq::ZmqClientProcess::send(GenericSendMessage &message, EventHandler &handler) {
    switch (message.type) {
    case None: {
        // exception
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
}

void BioTracker::Core::Zmq::ZmqClientProcess::shutdown() {
    isFree = true;
}

// =========================================
// PRIVATE
// =========================================

void BioTracker::Core::Zmq::ZmqClientProcess::track(SendTrackMessage &message, EventHandler &handler) {
    sendMessage(message);
    listenToEvents(handler);
}

void BioTracker::Core::Zmq::ZmqClientProcess::paint(SendPaintMessage &message, EventHandler &handler) {
    ReceivePaintMessage result(message);
    sendMessage(message);
    listenToEvents(handler);
    requestResults(result);
}

void BioTracker::Core::Zmq::ZmqClientProcess::paintOverlay(SendPaintOverlayMessage &message, EventHandler &handler) {
    ReceiveQPainterMessage result(message);
    sendMessage(message);
    listenToEvents(handler);
    requestResults(result);
}

void BioTracker::Core::Zmq::ZmqClientProcess::requestTools(SendRequestWidgetsMessage &message, EventHandler &handler) {
    ReceiveToolsWidgetMessage result(message);
    sendMessage(message);
    listenToEvents(handler);
    requestResults(result);
}

void BioTracker::Core::Zmq::ZmqClientProcess::buttonClicked(SendButtonClickMessage &message, EventHandler &handler) {
    sendMessage(message);
    listenToEvents(handler);
}

void BioTracker::Core::Zmq::ZmqClientProcess::sendValue(SendValueChangedMessage &message, EventHandler &handler) {
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
        zmq_send(m_socket, m.data, m.sizeInBytes, flag);
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

}

/**
 * Receive the message that is specified by the ReceiverMessage
 * @param message
 */
void ZmqClientProcess::requestResults(GenericReceiveMessage &message) {

}

// ===========================================
// ===========================================

void BioTracker::Core::Zmq::ZmqClientProcess::processBadError(QProcess::ProcessError error) {

}

void BioTracker::Core::Zmq::ZmqClientProcess::processError() {

}

}
}
}
