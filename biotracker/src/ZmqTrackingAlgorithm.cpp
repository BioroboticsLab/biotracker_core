#include "../ZmqTrackingAlgorithm.h"

namespace BioTracker {
namespace Core {
namespace Zmq {

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

void send_string(void *socket, QString str) {
    const int msg_size = str.length();
    int rc = 0;
    zmq_msg_t msg;
    rc = zmq_msg_init_size(&msg, msg_size);
    assert(rc == 0);
    memcpy(zmq_msg_data(&msg), str.toUtf8().constData(), msg_size);
    zmq_msg_send(&msg, socket, 0);
    assert(rc == 0);
    rc = zmq_msg_close(&msg);
    assert(rc == 0);

}

void send_cvMat(void *socket, const cv::Mat &mat) {
    QString data = QString::number(mat.cols) + "," + QString::number(
                       mat.rows) + "," + QString::number(mat.type());
    send_string(socket, data);
    //TODO error handling
    size_t sizeInBytes = mat.total() * mat.elemSize();
    zmq_msg_t msg;
    zmq_msg_init_size(&msg, sizeInBytes);
    memcpy(zmq_msg_data(&msg), mat.data, sizeInBytes);
    zmq_msg_send(&msg, socket, 0);
    zmq_msg_close(&msg);
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

    QProcess *zmqClient = new QProcess(this);
    //zmqClient->setStandardOutputFile("/example_tracker_py/out.txt");
    //zmqClient->setStandardErrorFile("/example_tracker_py/err.txt");
    zmqClient->setProcessChannelMode(QProcess::ForwardedChannels);
    QString command = info.m_program + " " + info.m_arguments.first();
    std::cout << "[exec]:" << command.toUtf8().constData() << std::endl;
    zmqClient->start(command);

    //cv::Mat E = cv::Mat::eye(4,4, CV_8UC1);
    //send_cvMat(m_socket, E);

    //cv::Mat E2 = cv::Mat::eye(4,4, CV_8UC3);
    //send_cvMat(m_socket, E2);

}

void ZmqTrackingAlgorithm::track(ulong frameNumber, const cv::Mat &frame) {
    std::cout << "track" << std::endl;
    send_cvMat(m_socket, frame);
    m_isTracking = true;
}

void ZmqTrackingAlgorithm::paint(ProxyPaintObject &, QPainter *p,
                                 const View &) {
    std::cout << "paint" << std::endl;
    std::cout << "paintOverlay" << std::endl;
    //p->setBrush(Qt::cyan);
    //p->setPen(Qt::darkCyan);
    //p->drawRect(0, 0, 100, 100);
    if (m_isTracking) {
        recv_QPainter(m_socket, p);
        m_isTracking = false;
    }
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
