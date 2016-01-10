#include "../ZmqTrackingAlgorithm.h"

#include "biotracker/util/ZmqHelper.h"

namespace BioTracker {
namespace Core {
namespace Zmq {

const QString TYPE_TRACK("0");
const QString TYPE_PAINT("1");
const QString TYPE_SHUTDOWN("2");
const QString TYPE_PAINTOVERLAY("3");
const QString TYPE_REQUEST_WIDGETS("4");
const QString TYPE_SEND_WIDGET_EVENT("5");

const QString WIDGET_EVENT_CLICK("0");
const QString WIDGET_EVENT_CHANGED("1");

inline QString senderId(QObject *obj) {
    QPushButton *btn = qobject_cast<QPushButton *>(obj);
    return btn->accessibleName();
}

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
    m_zmqMutex.lock();
    send_string(m_socket, TYPE_SHUTDOWN, 0);
    QThread::msleep(500);
    m_zmqMutex.unlock();
    m_zmqClient->kill();
    m_zmqClient->waitForFinished(1000);
    //zmq_close(m_socket);
    //zmq_ctx_term(m_context);
}

void ZmqTrackingAlgorithm::track(ulong frameNumber, const cv::Mat &frame) {
    m_zmqMutex.lock();
    send_string(m_socket, TYPE_TRACK, ZMQ_SNDMORE);
    send_mat(m_socket, frame, frameNumber);
    m_isTracking = true;
    m_zmqMutex.unlock();
}

void ZmqTrackingAlgorithm::paint(ProxyMat &m, const View &) {
    m_zmqMutex.lock();
    send_string(m_socket, TYPE_PAINT, ZMQ_SNDMORE);
    QString data = QString::number(99);
    send_string(m_socket, data, 0);
    // wait for reply
    QString flag = recv_string(m_socket);
    if (QString::compare(flag, "Y") == 0) {
        recv_mat(m_socket, m.getMat());
    }
    m_zmqMutex.unlock();
}

void ZmqTrackingAlgorithm::paintOverlay(QPainter *p, const View &) {
    m_zmqMutex.lock();
    send_string(m_socket, TYPE_PAINTOVERLAY, 0);
    recv_QPainter(m_socket, p);
    m_zmqMutex.unlock();
}

std::shared_ptr<QWidget> ZmqTrackingAlgorithm::getToolsWidget() {
    //zmqserver_requestWidgets(m_socket, m_tools, btns, txts, sliders, m_zmqMutex);
    m_zmqMutex.lock();

    // request widgets from client
    send_string(m_socket, TYPE_REQUEST_WIDGETS, 0);

    QVBoxLayout *mainLayout = new QVBoxLayout(m_tools.get()); // LEAK_CHECK?

    // wait for response
    // Elements are defined at:
    // https://github.com/BioroboticsLab/biotracker_core/wiki/dev_zmq#possible-elements
    QString data = recv_string(m_socket);
    std::cout << "data:" << data.toUtf8().data() << std::endl;
    if (data.length() > 0) {
        QStringList batch = data.split(";");
        for (auto &widgetStr : batch) {
            const QChar type = widgetStr.at(0);
            widgetStr.chop(1); // remove trailing ')'
            widgetStr = widgetStr.right(widgetStr.length() - 2); // remove first '_('
            auto widgetElems = widgetStr.split(",");
            const int uniqueId = widgetElems[0].toInt();
            if (type == QChar('d')) {

            } else if (type == QChar('t')) {
                QLabel *txt = new QLabel(m_tools.get());
                std::cout << "label?" << std::endl;
                txt->setText(widgetElems[1]);
                mainLayout->addWidget(txt);
            } else if (type == QChar('b')) {
                QPushButton *btn = new QPushButton(m_tools.get()); // LEAK_CHECK?
                btn->setText(widgetElems[1]);
                btn->setAccessibleName(QString::number(uniqueId));
                mainLayout->addWidget(btn);
                QObject::connect(btn, &QPushButton::clicked, this, &ZmqTrackingAlgorithm::btnClicked);
            } else if (type == QChar('s')) {

            } else {

            }
        }
    }
    m_zmqMutex.unlock();
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

void ZmqTrackingAlgorithm::zmqserverRequestWidgets() {

}
// == EVENTS ==

void ZmqTrackingAlgorithm::btnClicked() {
    const QString widgetId = senderId(sender());
    QString message;
    message.append(WIDGET_EVENT_CLICK);
    message.append(",");
    message.append(widgetId);
    m_zmqMutex.lock();
    send_string(m_socket, TYPE_SEND_WIDGET_EVENT, ZMQ_SNDMORE);
    send_string(m_socket, message, 0);
    m_zmqMutex.unlock();
}

// == EVENTS end ==

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
