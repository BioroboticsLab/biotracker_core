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

const QString EVENT_STOP_LISTENING("99");
const QString EVENT_NOTIFY_GUI("0");
const QString EVENT_UPDATE("1");
const QString EVENT_FORCE_TRACKING("2");
const QString EVENT_JUMP_TO_FRAME("3");
const QString EVENT_PLAY_PAUSE("4");

const QString EVENT_MSG_FALSE("0");

inline QString getSenderId(QObject *obj) {
    QWidget *w = qobject_cast<QWidget *>(obj);
    return w->accessibleName();
}

inline void setSenderId(QWidget *w, const QString id) {
    w->setAccessibleName(id);
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
    m_zmqMutex.lock();
    send_string(m_socket, TYPE_SHUTDOWN, 0);
    this->listenToEvents();
    QThread::msleep(500);
    m_zmqMutex.unlock();
    m_zmqClient->kill();
    m_zmqClient->waitForFinished(1000);
    m_zmqMutex.unlock();
    zmq_disconnect(m_socket, "172.0.0.1:5556");
    zmq_close(m_socket);
    zmq_ctx_term(m_context);
}

void ZmqTrackingAlgorithm::track(ulong frameNumber, const cv::Mat &frame) {
    m_zmqMutex.lock();
    send_string(m_socket, TYPE_TRACK, ZMQ_SNDMORE);
    send_mat(m_socket, frame, frameNumber);
    this->listenToEvents();
    m_isTracking = true;
    m_zmqMutex.unlock();
}

void ZmqTrackingAlgorithm::paint(ProxyMat &m, const View &) {
    m_zmqMutex.lock();
    send_string(m_socket, TYPE_PAINT, ZMQ_SNDMORE);
    QString data = QString::number(99);
    send_string(m_socket, data, 0);
    this->listenToEvents();
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
    this->listenToEvents();
    recv_QPainter(m_socket, p);
    m_zmqMutex.unlock();
}

std::shared_ptr<QWidget> ZmqTrackingAlgorithm::getToolsWidget() {
    //zmqserver_requestWidgets(m_socket, m_tools, btns, txts, sliders, m_zmqMutex);
    m_zmqMutex.lock();

    // request widgets from client
    send_string(m_socket, TYPE_REQUEST_WIDGETS, 0);
    this->listenToEvents();

    QVBoxLayout *mainLayout = new QVBoxLayout(m_tools.get()); // LEAK_CHECK?

    // wait for response
    // Elements are defined at:
    // https://github.com/BioroboticsLab/biotracker_core/wiki/dev_zmq#possible-elements
    QString data = recv_string(m_socket);
    if (data.length() > 0) {
        QStringList batch = data.split(";");
        for (auto &widgetStr : batch) {
            const QChar type = widgetStr.at(0);
            widgetStr.chop(1); // remove trailing ')'
            widgetStr = widgetStr.right(widgetStr.length() - 2); // remove first '_('
            auto widgetElems = widgetStr.split(",");
            const QString uniqueId = widgetElems[0];
            if (type == QChar('d')) {

            } else if (type == QChar('t')) {
                QLabel *txt = new QLabel(m_tools.get());
                txt->setText(widgetElems[1]);
                mainLayout->addWidget(txt);
            } else if (type == QChar('b')) {
                QPushButton *btn = new QPushButton(m_tools.get()); // LEAK_CHECK?
                btn->setText(widgetElems[1]);
                setSenderId(btn, uniqueId);
                mainLayout->addWidget(btn);
                QObject::connect(btn, &QPushButton::clicked, this, &ZmqTrackingAlgorithm::btnClicked);
            } else if (type == QChar('s')) {
                const int min = widgetElems[2].toInt();
                const int max = widgetElems[3].toInt();
                const int defaultValue = widgetElems[4].toInt();
                QLabel *txt = new QLabel(m_tools.get());
                txt->setText(widgetElems[1]);
                mainLayout->addWidget(txt);
                QSlider *sld = new QSlider(Qt::Horizontal, m_tools.get());
                setSenderId(sld, uniqueId);
                sld->setMinimum(min);
                sld->setMaximum(max);
                sld->setValue(defaultValue);
                mainLayout->addWidget(sld);
                QObject::connect(sld, &QSlider::valueChanged, this, &ZmqTrackingAlgorithm::sldValueChanged);
            } else {
                // silently ignore
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

/**
 * @brief ZmqTrackingAlgorithm::listenToEvents
 * CAREFUL, this function is NOT THREADSAFE
 */
void ZmqTrackingAlgorithm::listenToEvents() {
    QString event = recv_string(m_socket);
    while (event != EVENT_STOP_LISTENING) {
        // handle event
        if (event == EVENT_UPDATE) {
            Q_EMIT update();
        } else if (event == EVENT_FORCE_TRACKING) {
            Q_EMIT forceTracking();
        } else {
            const QStringList eventParts = event.split(",");
            if (eventParts[0] == EVENT_NOTIFY_GUI) {
                QString message = eventParts[1];
                // as the ',' and ';' characters are special symbols
                // for the zmq message transmission they get encoded
                // so they dont mess with the zmq message. Here we need
                // to restore them
                message = message.replace("%2C", ",");
                message = message.replace("%3B", ";");

                const size_t mtypeInt = eventParts[2].toInt();
                const MSGS::MTYPE mtype = MSGS::fromInt(mtypeInt);
                Q_EMIT notifyGUI(message.toStdString(), mtype);
            } else if (eventParts[0] == EVENT_JUMP_TO_FRAME) {
                const size_t frame = eventParts[1].toInt();
                Q_EMIT jumpToFrame(frame);
            } else if (eventParts[0] == EVENT_PLAY_PAUSE) {
                if (eventParts[1] == EVENT_MSG_FALSE) {
                    Q_EMIT pausePlayback(false);
                } else {
                    Q_EMIT pausePlayback(true);
                }
            } else {
                assert(false);
            }
        }
        event = recv_string(m_socket);
    }
}

// == EVENTS ==

void ZmqTrackingAlgorithm::btnClicked() {
    const QString widgetId = getSenderId(sender());
    QString message;
    message.append(WIDGET_EVENT_CLICK);
    message.append(",");
    message.append(widgetId);
    m_zmqMutex.lock();
    send_string(m_socket, TYPE_SEND_WIDGET_EVENT, ZMQ_SNDMORE);
    send_string(m_socket, message, 0);
    this->listenToEvents();
    m_zmqMutex.unlock();
}

void ZmqTrackingAlgorithm::sldValueChanged(int value) {
    const QString widgetId = getSenderId(sender());
    QString message;
    message.append(WIDGET_EVENT_CHANGED);
    message.append(",");
    message.append(widgetId);
    message.append(",");
    message.append(QString::number(value));
    m_zmqMutex.lock();
    send_string(m_socket, TYPE_SEND_WIDGET_EVENT, ZMQ_SNDMORE);
    send_string(m_socket, message, 0);
    this->listenToEvents();
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
