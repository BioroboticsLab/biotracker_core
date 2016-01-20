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

#include <zmq.h>

#include "settings/Messages.h"
#include "settings/ParamNames.h"
#include "serialization/TrackedObject.h"
#include "TrackingThread.h"
#include "Exceptions.h"

#include "zmq/ZmqInfoFile.h"

namespace BioTracker {
namespace Core {
namespace Zmq {

class ZmqTrackingAlgorithm : public TrackingAlgorithm {

  public:
    ZmqTrackingAlgorithm(ZmqInfoFile info, Settings &settings);

    virtual ~ZmqTrackingAlgorithm() override;

    void track(ulong frameNumber, const cv::Mat &frame) override;

    void paint(ProxyMat &, const View &) override;

    void paintOverlay(QPainter *, const View &) override;

    std::shared_ptr<QWidget> getToolsWidget() override;

    //    std::set<Qt::Key> &grabbedKeys() const override;

    void prepareSave() override;

    void postLoad() override;

    void postConnect() override;

  private:
    void mouseMoveEvent(QMouseEvent *) override;

    void mousePressEvent(QMouseEvent *) override;

    void mouseWheelEvent(QWheelEvent *) override;

    void keyPressEvent(QKeyEvent *) override;

    void listenToEvents();

    void shutdown();

  private Q_SLOTS:
    void btnClicked();
    void sldValueChanged(int value);
    void processError(QProcess::ProcessError error);
    void processHasError();

  private:
    std::unique_ptr<QProcess> m_zmqClient;
    bool m_isDead; // if this is true: its OVER
    bool m_isTracking;
    std::set<Qt::Key> m_keys;
    void *m_context;
    void *m_socket;
    std::shared_ptr<QWidget> m_tools;
    std::mutex m_zmqMutex;
};
}
}
}
