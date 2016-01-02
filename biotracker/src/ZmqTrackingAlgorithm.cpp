#include "../ZmqTrackingAlgorithm.h"

namespace BioTracker {
namespace Core {
namespace Zmq {

const QString TYPE_TRACK("0");
const QString TYPE_PAINT("1");
const QString TYPE_SHUTDOWN("2");
const QString TYPE_PAINTOVERLAY("3");
const QString TYPE_REQUEST_WIDGETS("4");
const QString TYPE_SEND_WIDGET_EVENT("5");

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
    free(string);
    return result;

}

void recv_mat(void *socket, cv::Mat &mat) {
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

void recv_ToolsWidgets(void *socket, std::shared_ptr<QWidget> tools) {
    std::cout << "get tools recv" << std::endl;
    QString data = recv_string(socket);
    std::cout << data.toUtf8().data() << std::endl;
    if (data.length() > 0) {
        QStringList batch = data.split(";");
        for (auto &widgetStr : batch) {
            const QChar type = widgetStr.at(0);
            if (type == QChar('d')) {

            } else if (type == QChar('t')) {

            } else if (type == QChar('b')) {
                auto btn = new QPushButton(tools.get());
                QString btnTxtDirty = widgetStr.split(",")[1];
                QString btnTxt = btnTxtDirty.left(btnTxtDirty.size() - 1);
                btn->setText(btnTxt);
            } else if (type == QChar('s')) {

            } else {

            }
        }
    }
}

void send_string(void *socket, QString str, int flags) {
    zmq_send(socket, str.toUtf8().constData(), str.length(), flags);
}

/**
 * @brief zmq_track
 * @param socket
 * @param mat
 * @param frame
 */
void zmqserver_track(void *socket, const cv::Mat &mat, const size_t frame, std::mutex &mut) {
    mut.lock();
    send_string(socket, TYPE_TRACK, ZMQ_SNDMORE);
    QString data = QString::number(mat.cols) + "," + QString::number(
                       mat.rows) + "," + QString::number(mat.type()) + "," + QString::number(frame);
    send_string(socket, data, ZMQ_SNDMORE);
    //TODO error handling
    size_t sizeInBytes = mat.total() * mat.elemSize();
    zmq_send(socket, mat.data, sizeInBytes, 0);
    mut.unlock();
}

void zmqserver_shutdown(void *socket, std::mutex &mut) {
    mut.lock();
    send_string(socket, TYPE_SHUTDOWN, 0);
    QThread::msleep(500);
    mut.unlock();
}

void zmqserver_requestWidgets(void *socket, std::shared_ptr<QWidget> tools, std::mutex &mut) {
    mut.lock();
    send_string(socket, TYPE_REQUEST_WIDGETS, 0);
    recv_ToolsWidgets(socket, tools);
    mut.unlock();
}

void zmqserver_paint(void *socket, const size_t frame, cv::Mat &m, std::mutex &mut) {
    mut.lock();
    send_string(socket, TYPE_PAINT, ZMQ_SNDMORE);
    QString data = QString::number(frame);
    send_string(socket, data, 0);

    // wait for reply
    QString flag = recv_string(socket);
    if (QString::compare(flag, "Y") == 0) {
        recv_mat(socket, m);
    }
    mut.unlock();
}

void zmqserver_paintOverlay(void *socket, QPainter *p, std::mutex &mut) {
    mut.lock();
    send_string(socket, TYPE_PAINTOVERLAY, 0);
    recv_QPainter(socket, p);
    mut.unlock();
}

// ==============================================

ZmqTrackingAlgorithm::ZmqTrackingAlgorithm(ZmqInfoFile info, Settings &settings) :
    TrackingAlgorithm(settings),
    m_isTracking(false),
    m_context(zmq_ctx_new()),
    m_socket(zmq_socket(m_context, ZMQ_PAIR)),
    m_tools(std::make_shared<QFrame>()) {
    int rc = zmq_bind(m_socket, "tcp://127.0.0.1:5556");
    if (rc != 0) {
        int rno = zmq_errno();
        QString err_msg(zmq_strerror(rno));
        std::cout << "error: " << err_msg.toUtf8().constData() << std::endl;
    }
    assert(rc == 0);
    m_zmqClient = std::make_unique<QProcess>(this);
    m_zmqClient->setProcessChannelMode(QProcess::ForwardedChannels);
    QString command = info.m_program + " " + info.m_arguments.first();
    m_zmqClient->start(command);
}

ZmqTrackingAlgorithm::~ZmqTrackingAlgorithm() {
    //zmq_disconnect(m_socket, "172.0.0.1:5556");
    zmqserver_shutdown(m_socket, m_zmqMutex);
    m_zmqClient->kill();
    m_zmqClient->waitForFinished(1000);
    //zmq_close(m_socket);
    //zmq_ctx_term(m_context);
}

void ZmqTrackingAlgorithm::track(ulong frameNumber, const cv::Mat &frame) {
    zmqserver_track(m_socket, frame, frameNumber, m_zmqMutex);
    m_isTracking = true;
}

void ZmqTrackingAlgorithm::paint(cv::Mat &m, const View &) {
    zmqserver_paint(m_socket, 0, m, m_zmqMutex);
}

void ZmqTrackingAlgorithm::paintOverlay(QPainter *p, const View &) {
    zmqserver_paintOverlay(m_socket, p, m_zmqMutex);
}

std::shared_ptr<QWidget> ZmqTrackingAlgorithm::getToolsWidget() {
    zmqserver_requestWidgets(m_socket, m_tools, m_zmqMutex);
    //send_string(m_socket, TYPE_REQUEST_WIDGETS, 0);
    //recv_ToolsWidgets(m_socket, m_tools);
    return m_tools;
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
