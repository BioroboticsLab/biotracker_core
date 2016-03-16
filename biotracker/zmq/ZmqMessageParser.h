#pragma once

#include <QString>
#include <memory>
#include <condition_variable>
#include <atomic>
#include <QRect>
#include <QPainter>
#include <QPoint>
#include <zmq.h>
#include <opencv2/opencv.hpp>
#include <QProcess>
#include "biotracker/TrackingAlgorithm.h"
#include "biotracker/zmq/ZmqInfoFile.h"
#include "biotracker/settings/Messages.h"

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

const QString FAILED_RECV_STRING("FAILEDRECVSTR");


const cv::Mat DEFAULT_1x1(1,1, CV_8UC3);

// TODO solve this more elegant with polymorphsm
class ZmqMessage {
  public:

    ZmqMessage(QString str):
        isText(true),
        text(str),
        mat(DEFAULT_1x1) {
    }
    ZmqMessage(const cv::Mat &m):
        isText(false),
        mat(m) {}

    const bool isText;
    const QString text;
    const cv::Mat &mat;
};

typedef QString(*recvString)();
typedef void (*recvMat)(cv::Mat &mat);

// ======================================
// EVENT HANDLER
// ======================================

class EventHandler : public QObject {
    Q_OBJECT
  public:
    EventHandler(QObject *p): QObject(p) {}
    void receive(recvString receiveStr);
  Q_SIGNALS:
    void notifyGUI(std::string, MSGS::MTYPE type = MSGS::MTYPE::NOTIFICATION);
    void update();
    // cv::Mat requestCurrentScreen();
    void forceTracking();
    //void registerViews(const std::vector<View> views);
    void pausePlayback(bool paused);
    void jumpToFrame(int frameNumber);

    void btnClicked(const QString widgetId);
    void sldValueChanged(const QString widgetId, int value);
  public Q_SLOTS:
    void btnClickedInternal();
    void sldValueChangedInternal(int value);

};

// ======================================
// GENERIC
// ======================================

enum MessageType {
    None,
    Track,
    Paint,
    PaintOverlay,
    RequestTools,
    ButtonClick,
    ValueChanged
};

class GenericSendMessage {
  public:
    GenericSendMessage(const MessageType t): type(t) {}
    virtual ~GenericSendMessage() {}

    /**
     * @brief get
     * @return the message parts that will be send to the client
     */
    virtual std::vector<ZmqMessage> get() = 0;

    const MessageType type;

  protected:
    ZmqMessage fromString(const QString str);
};

// ======================================

/**
 * @brief The GenericZmqMessage class
 * Encapsulation of the message logic for the zmq client communication
 */
class GenericReceiveMessage {
  public:
    virtual ~GenericReceiveMessage() {}

    /**
     * @brief receive
     * A message part is received and must be processed
     * @param m
     * @return true -> keep receiving, false -> stop receiving
     */
    virtual void receive(recvString receiveStr, recvMat recvMat) = 0;
};


// ======================================
// TRACK
// ======================================

/**
 * @brief The SendTrackMessage class
 * Use this message to send a Track request to the client
 */
class SendTrackMessage : public GenericSendMessage {

  public:
    SendTrackMessage(const cv::Mat &frame, ulong frameNumber)
        : GenericSendMessage(Track),
          m_frame(frame),
          m_frameNumber(frameNumber) {}

    std::vector<ZmqMessage> get() override;

  protected:
    const cv::Mat &m_frame;
    const ulong m_frameNumber;
};

// ======================================
// PAINT
// ======================================

/**
 * @brief The SendPaintMessage class
 */
class SendPaintMessage: public GenericSendMessage {
  public:
    SendPaintMessage(const int fnr, const std::string v, cv::Mat &m):
        GenericSendMessage(Paint), m_mat(m), m_frameNumber(fnr), m_view(v) {}

    std::vector<ZmqMessage> get() override;
    cv::Mat &m_mat;
  protected:
    const int m_frameNumber;
    const std::string m_view;
};

// ======================================

class ReceivePaintMessage : public GenericReceiveMessage {
  public:
    ReceivePaintMessage(SendPaintMessage &sending):
        m_mat(sending.m_mat),
        m_firstCall(true),
        m_secondCall(false) {}

    void receive(recvString receiveStr, recvMat recvMat) override;

  private:
    cv::Mat &m_mat;
    int m_w;
    int m_h;
    int m_type;
    bool m_firstCall;
    bool m_secondCall;
};

// ======================================
// PAINTOVERLAY
// ======================================

class SendPaintOverlayMessage: public GenericSendMessage {
  public:
    SendPaintOverlayMessage(const std::string v, QPainter *p):
        GenericSendMessage(PaintOverlay), m_painter(p), m_view(v) {}

    std::vector<ZmqMessage>get() override;

    QPainter *m_painter;
  protected:
    const std::string m_view;
};

// ======================================

class ReceiveQPainterMessage: public GenericReceiveMessage {
  public:
    ReceiveQPainterMessage(SendPaintOverlayMessage &parent):
        m_painter(parent.m_painter) {}

    void receive(recvString receiveStr, recvMat recvMat) override;

  private:
    QPainter *m_painter;
};

// ======================================
// REQUESTWIDGETS
// ======================================

class SendRequestWidgetsMessage: public GenericSendMessage {
  public:
    SendRequestWidgetsMessage(QPointer<QWidget> tools):
        GenericSendMessage(RequestTools),
        m_tools(tools) {}
    std::vector<ZmqMessage>get() override;

    QPointer<QWidget> m_tools;
};

// ======================================

class ReceiveToolsWidgetMessage: public GenericReceiveMessage {
  public:
    ReceiveToolsWidgetMessage(SendRequestWidgetsMessage &m, EventHandler &evt):
        m_tools(m.m_tools), m_events(evt) {}

    void receive(recvString receiveStr, recvMat recvMat) override;

  private:
    QPointer<QWidget> m_tools;
    EventHandler &m_events;
};

// ======================================
// BUTTONCLICK
// ======================================

class SendButtonClickMessage: public GenericSendMessage {
  public:
    SendButtonClickMessage(const QString s):
        GenericSendMessage(ButtonClick), m_sender(s) {}
    std::vector<ZmqMessage>get() override;
  private:
    const QString m_sender;
};

// ======================================
// VALUECHANGED
// ======================================

class SendValueChangedMessage: public GenericSendMessage {
  public:
    SendValueChangedMessage(QString s, const int v):
        GenericSendMessage(ValueChanged),
        m_sender(s),
        m_value(v) {}
    std::vector<ZmqMessage>get() override;
  private:
    const QString m_sender;
    const int m_value;
};


}
}
}
