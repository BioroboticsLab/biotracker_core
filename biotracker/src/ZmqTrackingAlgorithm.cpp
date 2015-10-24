#include "../ZmqTrackingAlgorithm.h"

namespace BioTracker {
namespace Core {
namespace Zmq {

// ==============================================
// P R I V A T E  Z M Q  H E L P E R  F U N C S
// ==============================================

zmq_msg_t recv_msg(void *socket) {
    int rc;
    zmq_msg_t msg;
    rc = zmq_msg_init(&msg);
    assert(rc == 0);
    int bytes = zmq_msg_recv(&msg, socket, 0);
    return msg;
}

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
    return result;

}

// ==============================================

ZmqTrackingAlgorithm::ZmqTrackingAlgorithm(ZmqInfoFile &info,
        Settings &settings, QWidget *parent) :
    TrackingAlgorithm(settings, parent),
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
    zmqClient->setProcessChannelMode(QProcess::MergedChannels);
    zmqClient->start(info.m_program + " " + info.m_arguments.first());
    zmqClient->waitForFinished();
    QByteArray o = zmqClient->readAllStandardOutput();
    QString _o(o);

    std::cout << "cmd " << _o.toUtf8().constData() << std::endl;

    //zmq::message_t funcStructure(100);
    //memset(funcStructure.data(), 0, 100);
    //m_socket.recv(&funcStructure);

    QString string = recv_string(m_socket);

    std::cout << "receive string:" << string.toUtf8().constData() << std::endl;

}

void ZmqTrackingAlgorithm::track(ulong frameNumber, const cv::Mat &frame) {
    std::cout << "track" << std::endl;
}

void ZmqTrackingAlgorithm::paint(ProxyPaintObject &, QPainter *p, const View &) {

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
