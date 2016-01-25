#include "biotracker/zmq/ZmqProcessHandler.h"

namespace BioTracker {
namespace Core {
namespace Zmq {

/**
  CTOR
 * @brief ZmqProcessHandler::ZmqProcessHandler
 * @param parent
 */
ZmqProcessHandler::ZmqProcessHandler():
    m_context(zmq_ctx_new()) {
    std::cout << "start handler2" << std::endl;

}

/**
  DTOR
 * @brief ZmqProcessHandler::~ZmqProcessHandler
 */
ZmqProcessHandler::~ZmqProcessHandler() {
    if (m_currentProcess) {
        m_currentProcess->shutdown();
        m_currentProcess.reset();
    }
    zmq_ctx_term(m_context);
}

std::shared_ptr<ZmqClientProcess> ZmqProcessHandler::startProcess(ZmqInfoFile &info) {
    if (m_currentProcess) {
        m_currentProcess->shutdown();
        m_currentProcess.reset();
    }

    void *socket = zmq_socket(m_context, ZMQ_PAIR);
    m_currentProcess = std::make_shared<ZmqClientProcess>(info, socket);

    return m_currentProcess;
}



}
}
}
