#pragma once

#include <QString>
#include <QRect>
#include <QPainter>
#include <QPoint>
#include <zmq.h>
#include <opencv2/opencv.hpp>

#include "ZmqMessageParser.h"

/**
 * Zmq helper functions
 *
 * This functions are NOT THREADSAFE!
 * USERS MUST TAKE CARE THAT CALLS ARE PROTECTED BY MUTEX
 *
 */
namespace BioTracker {
namespace Core {
namespace Zmq {


/**
 * @brief recv_string
 *    receives a string from the given socket
 * @param socket
 * @return
 */
inline QString recv_string(void *socket) {
    zmq_msg_t msg;
    int rc = zmq_msg_init(&msg);
    assert(rc == 0);
    const int bytes = zmq_msg_recv(&msg, socket, 0);
    auto string = FAILED_RECV_STRING;
    if (bytes > 0) {
        string = QString::fromLocal8Bit(static_cast<char *>(zmq_msg_data(&msg)), bytes);
    }
    zmq_msg_close(&msg);
    return string;
}

/**
 * @brief send_string
 *   sends a string to the given socket
 * @param socket
 * @param str
 * @param flags
 */
inline void send_string(void *socket, QString str, int flags=0) {
    zmq_send(socket, str.toUtf8().constData(), str.length(), flags);
}

/**
 * @brief recv_mat
 *    receives a cv::Mat from the given socket
 * @param socket
 * @param mat
 */
inline void recv_mat(void *socket, cv::Mat &mat) {
    int rc = 0;
    QString temp_shape = recv_string(socket);
    QStringRef shape(&temp_shape);
    QVector<QStringRef> shapeStr = shape.split(",");
    const int w = shapeStr.at(0).toInt();
    const int h = shapeStr.at(1).toInt();
    const int type = shapeStr.at(2).toInt();
    zmq_msg_t msg;
    rc = zmq_msg_init(&msg);
    assert(rc == 0);
    zmq_msg_recv(&msg, socket, 0);
    void *msg_content = zmq_msg_data(&msg);
    cv::Mat newMat(h, w, type, msg_content);
    newMat.copyTo(mat);
    zmq_msg_close(&msg);
}

inline void send_mat(void *socket, const cv::Mat &mat, const size_t frameNbr) {
    QString data = QString::number(mat.cols) + "," + QString::number(
                       mat.rows) + "," + QString::number(mat.type()) + "," + QString::number(frameNbr);
    send_string(socket, data, ZMQ_SNDMORE);
    //TODO error handling
    size_t sizeInBytes = mat.total() * mat.elemSize();
    zmq_send(socket, mat.data, sizeInBytes, 0);
}

}
}
}
