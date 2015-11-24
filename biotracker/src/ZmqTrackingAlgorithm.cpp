#include "../ZmqTrackingAlgorithm.h"

namespace BioTracker {
namespace Core {
namespace Zmq {

const QString TYPE_TRACK("0");
const QString TYPE_PAINT("1");
const QString TYPE_SHUTDOWN("2");

// ==============================================
// P R I V A T E  Z M Q  H E L P E R  F U N C S
// ==============================================

QString recv_string(void *socket) {
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
    zmq_msg_close(&msg);
    free(string);
    return result;

}

QColor getColor(const QStringRef content) {
    QVector<QStringRef> colorStr = content.split(",");
    const int r = colorStr.at(0).toInt();
    const int g = colorStr.at(1).toInt();
    const int b = colorStr.at(2).toInt();
    const int a = colorStr.at(3).toInt();
    QColor color(r, g, b, a);
    return color;
}

QRect getRect(const QStringRef content) {
    QVector<QStringRef> colorStr = content.split(",");
    const int x = colorStr.at(0).toInt();
    const int y = colorStr.at(1).toInt();
    const int w = colorStr.at(2).toInt();
    const int h = colorStr.at(3).toInt();
    QRect rect(x, y, w, h);
    return rect;
}

void recv_QPainter(void *socket, QPainter *p) {
    QString paintBatch = recv_string(socket);
    if (paintBatch.length() > 0) {
        QStringList batch = paintBatch.split(";");
        for (int i = 0; i < batch.size(); ++i) {
            const QString paintOperation = batch.at(i);
            int start = 2;
            int length = paintOperation.size() - 3;
            QStringRef content(&paintOperation, start, length);
            if (paintOperation.startsWith("p")) {
                p->setPen(getColor(content));
            } else if (paintOperation.startsWith("b")) {
                p->setBrush(getColor(content));
            } else if (paintOperation.startsWith("r")) {
                p->drawRect(getRect(content));
            }
        }
    }

}

void send_string(void *socket, QString str, int flags) {
    const int msg_size = str.length();
    int rc = 0;
    zmq_msg_t msg;
    rc = zmq_msg_init_size(&msg, msg_size);
    assert(rc == 0);
    memcpy(zmq_msg_data(&msg), str.toUtf8().constData(), msg_size);
    zmq_msg_send(&msg, socket, flags);
    assert(rc == 0);
    rc = zmq_msg_close(&msg);
    assert(rc == 0);

}

/**
 * @brief zmq_track
 * @param socket
 * @param mat
 * @param frame
 */
void zmqserver_track(void *socket, const cv::Mat &mat, const size_t frame) {
    send_string(socket, TYPE_TRACK, ZMQ_SNDMORE);
    QString data = QString::number(mat.cols) + "," + QString::number(
                       mat.rows) + "," + QString::number(mat.type()) + "," + QString::number(frame);
    send_string(socket, data, ZMQ_SNDMORE);
    //TODO error handling
    size_t sizeInBytes = mat.total() * mat.elemSize();
    zmq_msg_t msg;
    zmq_msg_init_size(&msg, sizeInBytes);
    memcpy(zmq_msg_data(&msg), mat.data, sizeInBytes);
    zmq_msg_send(&msg, socket, 0);
    zmq_msg_close(&msg);
}

void zmqserver_shutdown(void *socket) {
    send_string(socket, TYPE_SHUTDOWN, 0);
    QThread::msleep(500);
}

void zmqserver_paint(void *socket, const size_t frame, QPainter *p) {
    send_string(socket, TYPE_PAINT, ZMQ_SNDMORE);
    QString data = QString::number(frame);
    send_string(socket, data, 0);

    std::cout << "#1" << std::endl;

    // wait for reply
    QString flag = recv_string(socket);
    std::cout << "#2" << std::endl;
    recv_QPainter(socket, p);
    std::cout << "#3" << std::endl;
    if (QString::compare(flag, "Y") == 0) {
        std::cout << "#4" << flag.toUtf8().data() << std::endl;
        QString shape = recv_string(socket);
        std::cout << "#5" << std::endl;
        std::cout << "shape: " << shape.toUtf8().data() << std::endl;
        //auto data =
    }

}

// ==============================================

ZmqTrackingAlgorithm::ZmqTrackingAlgorithm(ZmqInfoFile &info,
        Settings &settings, QWidget *parent) :
    TrackingAlgorithm(settings, parent),
    m_isTracking(false),
    m_context(zmq_ctx_new()),
    m_socket(zmq_socket(m_context, ZMQ_PAIR)) {

    std::cout << info.m_arguments.first().toUtf8().constData() << std::endl;

    int rc = zmq_bind(m_socket, "tcp://127.0.0.1:5556");
    if (rc != 0) {
        int rno = zmq_errno();
        QString err_msg(zmq_strerror(rno));
        std::cout << "error: " << err_msg.toUtf8().constData() << std::endl;
    }
    assert(rc == 0);

    std::cout << "HERE" << std::endl;

    m_zmqClient = new QProcess(this);

    m_zmqClient->setProcessChannelMode(QProcess::ForwardedChannels);
    QString command = info.m_program + " " + info.m_arguments.first();
    std::cout << "[exec]:" << command.toUtf8().constData() << std::endl;
    m_zmqClient->start(command);
}

ZmqTrackingAlgorithm::~ZmqTrackingAlgorithm() {
    //zmq_disconnect(m_socket, "172.0.0.1:5556");
    zmqserver_shutdown(m_socket);
    m_zmqClient->kill();
    m_zmqClient->waitForFinished(1000);
    zmq_close(m_socket);
    zmq_ctx_term(m_context);
}

void ZmqTrackingAlgorithm::track(ulong frameNumber, const cv::Mat &frame) {
    std::cout << "track" << std::endl;
    //send_cvMat(m_socket, frame);
    zmqserver_track(m_socket, frame, frameNumber);
    m_isTracking = true;
}

void ZmqTrackingAlgorithm::paint(ProxyPaintObject &, QPainter *p,
                                 const View &) {
    std::cout << "paint" << std::endl;
    std::cout << "paintOverlay" << std::endl;
    zmqserver_paint(m_socket, 0, p);
}

std::shared_ptr<QWidget> ZmqTrackingAlgorithm::getToolsWidget() {
    auto ptr = std::make_shared<QWidget>();
    return ptr;
}

std::shared_ptr<QWidget> ZmqTrackingAlgorithm::getParamsWidget() {
    auto ptr = std::make_shared<QWidget>();
    return ptr;
}

void ZmqTrackingAlgorithm::prepareSave() {

}

void ZmqTrackingAlgorithm::postLoad() {

}

void ZmqTrackingAlgorithm::postConnect() {

}

void ZmqTrackingAlgorithm::mouseMoveEvent(QMouseEvent *) {

}

void ZmqTrackingAlgorithm::mousePressEvent(QMouseEvent *) {

}

void ZmqTrackingAlgorithm::mouseWheelEvent(QWheelEvent *) {

}

void ZmqTrackingAlgorithm::keyPressEvent(QKeyEvent *) {

}

ZmqInfoFile getInfo(const boost::filesystem::path &path) {

    if (!boost::filesystem::exists(path)) {
        throw file_not_found("Could not find file " + path.string());
    }

    QFile infoFile(QString::fromStdString(path.string()));

    if (!infoFile.open(QIODevice::ReadOnly)) {
        throw device_open_error("Could not open file " + path.string());
    }

    int c = 0;
    QString name = QString::fromStdString(path.stem().string());
    QString path_folder = QString::fromStdString(path.string());
    const int last = path_folder.length() - 1;
    const int first = last - path.filename().string().length();
    path_folder = path_folder.remove(first, last);

    QString program;
    QStringList arguments;

    QTextStream in(&infoFile);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (!line.startsWith("#")) {
            if (c == 0) {
                program = line;
                c += 1;
            } else if (c == 1) {
                arguments << path_folder + "/" + line;
                c+= 1;
                break;
            }
        }
    }
    if (c != 2) {
        throw device_open_error("Malformed *.zmq file: " + path.string());
    }

    ZmqInfoFile f(name, program, arguments);
    return f;
}


}
}
}
