#pragma once

#include <QString>
#include <QRect>
#include <QPainter>
#include <QPoint>
#include <zmq.h>
#include <boost/filesystem/operations.hpp>
#include <opencv2/opencv.hpp>

namespace BioTracker {
namespace Core {
namespace Zmq {

/**
 * @brief The ZmqInfoFile class
 * The zmq clients are represented in a simple text file with the
 * following structure:
 *
 *    * Any line with a # will be ignored
 *    * The frist none-comment line defines the program that will
 *      start the client (e.g. 'python')
 *    * The second none-comment line defines the zmq-client file
 *    * The file MUST end on '*.tracker.zmq'
 *
 */
class ZmqInfoFile {
  public:
    ZmqInfoFile(const boost::filesystem::path &path);
    QString m_name;
    QString m_program;
    QStringList m_arguments;
};

}
}
}
