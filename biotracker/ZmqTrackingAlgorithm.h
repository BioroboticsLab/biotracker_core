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

    ~ZmqTrackingAlgorithm();

    void track(ulong frameNumber, const cv::Mat &frame) override;

    void paint(ProxyPaintObject &, QPainter *, const View &) override;

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
    QProcess *m_zmqClient;
    bool m_isTracking;
    std::set<Qt::Key> m_keys;
    void *m_context;
    void *m_socket;
};
}
}
}
