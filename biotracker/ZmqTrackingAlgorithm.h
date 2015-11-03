#pragma once

#include <vector>
#include <memory>
#include <set>

#include <boost/optional.hpp>
#include <boost/filesystem/operations.hpp>
#include <opencv2/opencv.hpp>

#include <QFile>
#include <QWidget>
#include <QColor>
#include <QStringRef>
#include <QVector>
#include <QMouseEvent>

//#include <zmq.hpp>
#include <zmq.h>

#include "settings/Messages.h"
#include "settings/ParamNames.h"
#include "serialization/TrackedObject.h"
#include "TrackingThread.h"
#include "Exceptions.h"

namespace BioTracker {
namespace Core {
namespace Zmq {

class ZmqInfoFile {
  public:
    ZmqInfoFile(QString n, QString p, QStringList a):
        m_name(n),
        m_program(p),
        m_arguments(a) {}
    QString m_name;
    QString m_program;
    QStringList m_arguments;
};

/**
 * @brief getInfo
 *
 *
 *
 * @param path to the *.zmq file
 * @return the info provided in the *.zmq file
 */
ZmqInfoFile getInfo(const boost::filesystem::path &path);

class ZmqTrackingAlgorithm : public TrackingAlgorithm {

  public:
    ZmqTrackingAlgorithm(ZmqInfoFile &info, Settings &settings, QWidget *parent);

    ~ZmqTrackingAlgorithm() {
        zmq_disconnect(m_socket, "172.0.0.1:5556");
        zmq_close(m_socket);
        zmq_ctx_term(m_context);
    }

    void track(ulong frameNumber, const cv::Mat &frame) override;

    void paint(ProxyPaintObject &, const View &) override;

    void paintOverlay(QPainter *p) override;

    std::shared_ptr<QWidget> getToolsWidget() override;

    std::shared_ptr<QWidget> getParamsWidget() override;

    //    std::set<Qt::Key> &grabbedKeys() const override;

    void prepareSave() override;

    void postLoad() override;

    void postConnect() override;

  private:
    void mouseMoveEvent(QMouseEvent *) override;

    void mousePressEvent(QMouseEvent *) override;

    void mouseWheelEvent(QWheelEvent *) override;

    void keyPressEvent(QKeyEvent *) override;

  private:
    bool m_isTracking;
    std::set<Qt::Key> m_keys;
    void *m_context;
    void *m_socket;

};
}
}
}
