#include "zmq/ZmqProcessHandler.h"

namespace BioTracker {
namespace Core {
namespace Zmq {

ZmqProcessHandler::ZmqProcessHandler(QObject *p, const ZmqInfoFile info, void *socket):
    QObject(p),
    m_socket(socket) {

}

ZmqProcessHandler::~ZmqProcessHandler() {

}

void ZmqProcessHandler::stop() {

}

void ZmqProcessHandler::start() {

}

void ZmqProcessHandler::processBadError(QProcess::ProcessError error) {

}

void ZmqProcessHandler::processError() {

}



}
}
}
