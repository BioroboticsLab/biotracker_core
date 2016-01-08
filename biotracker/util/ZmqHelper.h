#pragma once

#include <QString>
#include <zmq.h>

/**
 * Zmq helper functions
 */
namespace BioTracker {
namespace Core {
namespace Zmq {

inline QString recv_string(void *socket) {
    int rc;
    zmq_msg_t msg;
    rc = zmq_msg_init(&msg);
    assert(rc == 0);
    int bytes = zmq_msg_recv(&msg, socket, 0);

    char *string = static_cast<char *>(malloc(bytes + 1));
    void *msg_content = zmq_msg_data(&msg);

    memcpy(string, msg_content, bytes);
    zmq_msg_close(&msg);
    string[bytes] = 0;
    QString result(string);
    free(string);
    return result;
}

inline void send_string(void *socket, QString str, int flags=0) {
    zmq_send(socket, str.toUtf8().constData(), str.length(), flags);
}

}
}
}
