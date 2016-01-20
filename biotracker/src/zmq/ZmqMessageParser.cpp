#include "zmq/ZmqMessageParser.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>

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

// ================================
// GenericSendMessage
// ================================

ZmqMessage GenericSendMessage::fromString(const QString &str) {
    ZmqMessage m(str.toUtf8().constData(), str.size());
    return m;
}

// ================================
// SendTrackMessage
// ================================

std::vector<ZmqMessage> SendTrackMessage::get() {
    std::vector<ZmqMessage> message;
    message.push_back(fromString(TYPE_TRACK)); // #1

    QString data = QString::number(m_frame.cols) + "," + QString::number(
                       m_frame.rows) + "," + QString::number(m_frame.type()) + "," +
                   QString::number(m_frameNumber);
    message.push_back(fromString(data)); // #2

    size_t sizeInBytes = m_frame.total() * m_frame.elemSize();
    message.push_back(ZmqMessage(m_frame.data, sizeInBytes)); // #3

    return message;
}

// ================================
// SendPaintMessage
// ================================

std::vector<ZmqMessage> SendPaintMessage::get() {
    std::vector<ZmqMessage> message;

    message.push_back(fromString(TYPE_PAINT)); // #1

    QString data = QString::number(m_frameNumber);
    message.push_back(fromString(data)); // #2

    return message;
}

// ================================
// SendPaintOverlayMessage
// ================================

std::vector<ZmqMessage> SendPaintOverlayMessage::get() {
    std::vector<ZmqMessage> message;
    message.push_back(fromString(TYPE_PAINTOVERLAY));
    return message;
}

// ================================
// SendRequestWidgetsMessage
// ================================

std::vector<ZmqMessage> SendRequestWidgetsMessage::get() {
    std::vector<ZmqMessage> message;
    message.push_back(fromString(TYPE_REQUEST_WIDGETS));
    return message;
}

// ================================
// SendButtonClickMessage
// ================================

std::vector<ZmqMessage> SendButtonClickMessage::get() {
    std::vector<ZmqMessage> message;
    message.push_back(fromString(TYPE_SEND_WIDGET_EVENT));

    QString click;
    click.append(WIDGET_EVENT_CLICK);
    click.append(",");
    click.append(m_sender);
    message.push_back(fromString(click));

    return message;
}

// ================================
// SendValueChangedMessage
// ================================

std::vector<ZmqMessage> SendValueChangedMessage::get() {
    std::vector<ZmqMessage> message;
    message.push_back(fromString(TYPE_SEND_WIDGET_EVENT));

    QString changed;
    changed.append(WIDGET_EVENT_CHANGED);
    changed.append(",");
    changed.append(m_sender);
    changed.append(",");
    changed.append(QString::number(m_value));
    message.push_back(fromString(changed));

    return message;
}

// ================================
// EventHandler
// ================================

const QString EVENT_STOP_LISTENING("99");
const QString EVENT_NOTIFY_GUI("0");
const QString EVENT_UPDATE("1");
const QString EVENT_FORCE_TRACKING("2");
const QString EVENT_JUMP_TO_FRAME("3");
const QString EVENT_PLAY_PAUSE("4");

const QString EVENT_MSG_FALSE("0");

void EventHandler::receive(BioTracker::Core::Zmq::recvString receiveStr) {
    auto string = receiveStr();
    while (string != EVENT_STOP_LISTENING) {
        if (string == EVENT_UPDATE) {
            Q_EMIT update();
        } else if (string == EVENT_FORCE_TRACKING) {
            Q_EMIT forceTracking();
        } else {
            const QStringList eventParts = string.split(",");
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
        string = receiveStr();
    }
}

// ================================
// ZmqMessage
// ================================

QString ZmqMessage::toString() {
    return QString::fromLocal8Bit(static_cast<const char *>(data), sizeInBytes);
}

// ================================
// ReceivePaintMessage
// ================================

/*
bool ReceivePaintMessage::receive(ZmqMessage m)
{
    const QString input = m.toString();
    if (m_firstCall) {
        m_firstCall = false;
        if (input == "Y") {
            return true;
        }
    } else if (m_secondCall) {
        // receive matrix shape
        QStringRef shape(&input);
        QVector<QStringRef> shapeStr = shape.split(",");
        m_w = shapeStr.at(0).toInt();
        m_h = shapeStr.at(1).toInt();
        m_type = shapeStr.at(2).toInt();
        return true;
    } else {
        // receive matrix data
        cv::Mat newMat(m_h, m_w, m_type, &m.data);
        newMat.copyTo(m_mat);
    }
    return false;
}
*/

void ReceivePaintMessage::receive(BioTracker::Core::Zmq::recvString receiveStr,
                                  BioTracker::Core::Zmq::recvMat recvMat) {
    QString input = receiveStr();
    if (input != "Y") {
        recvMat(m_mat);
        // receive matrix shape
        /*input = receiveStr();
        QStringRef shape(&input);
        QVector<QStringRef> shapeStr = shape.split(",");
        m_w = shapeStr.at(0).toInt();
        m_h = shapeStr.at(1).toInt();
        m_type = shapeStr.at(2).toInt();
        return true;
        // receive matrix data
        cv::Mat newMat(m_h, m_w, m_type, &m.data);
        newMat.copyTo(m_mat);*/
    }

}

// ================================
// ReceiveQPainterMessage
// ================================

QColor convertToColor(const QStringRef content) {
    QVector<QStringRef> colorStr = content.split(",");
    const int r = colorStr.at(0).toInt();
    const int g = colorStr.at(1).toInt();
    const int b = colorStr.at(2).toInt();
    const int a = colorStr.at(3).toInt();
    QColor color(r, g, b, a);
    return color;
}

QRect convertToRect(const QStringRef content) {
    QVector<QStringRef> colorStr = content.split(",");
    const int x = colorStr.at(0).toInt();
    const int y = colorStr.at(1).toInt();
    const int w = colorStr.at(2).toInt();
    const int h = colorStr.at(3).toInt();
    QRect rect(x, y, w, h);
    return rect;
}

void BioTracker::Core::Zmq::ReceiveQPainterMessage::receive(BioTracker::Core::Zmq::recvString receiveStr,
        BioTracker::Core::Zmq::recvMat) {
    QString paintBatch = receiveStr();
    if (paintBatch.length() > 0) {
        QStringList batch = paintBatch.split(";");
        for (int i = 0; i < batch.size(); ++i) {
            const QString paintOperation = batch.at(i);
            int start = 2;
            int length = paintOperation.size() - 3;
            QStringRef content(&paintOperation, start, length);
            if (paintOperation.startsWith("p")) {
                m_painter->setPen(convertToColor(content));
            } else if (paintOperation.startsWith("b")) {
                m_painter->setBrush(convertToColor(content));
            } else if (paintOperation.startsWith("r")) {
                m_painter->drawRect(convertToRect(content));
            }
        }
    }
}

// ================================
// ReceiveToolsWidgetsMessage
// ================================


void ReceiveToolsWidgetMessage::receive(BioTracker::Core::Zmq::recvString receiveStr,
                                        BioTracker::Core::Zmq::recvMat recvMat) {
    QVBoxLayout *mainLayout = new QVBoxLayout(m_tools.get()); // LEAK_CHECK?

    // wait for response
    // Elements are defined at:
    // https://github.com/BioroboticsLab/biotracker_core/wiki/dev_zmq#possible-elements
    QString data = receiveStr();
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
                btn->setAccessibleName(uniqueId);
                mainLayout->addWidget(btn);
                QObject::connect(btn, &QPushButton::clicked, m_buttonClickCallback);
            } else if (type == QChar('s')) {
                const int min = widgetElems[2].toInt();
                const int max = widgetElems[3].toInt();
                const int defaultValue = widgetElems[4].toInt();
                QLabel *txt = new QLabel(m_tools.get());
                txt->setText(widgetElems[1]);
                mainLayout->addWidget(txt);
                QSlider *sld = new QSlider(Qt::Horizontal, m_tools.get());
                sld->setAccessibleName(uniqueId);
                sld->setMinimum(min);
                sld->setMaximum(max);
                sld->setValue(defaultValue);
                mainLayout->addWidget(sld);
                QObject::connect(sld, &QSlider::valueChanged, m_sliderChangedCallback);
            } else {
                // silently ignore
            }
        }
    }
}

}
}
}
