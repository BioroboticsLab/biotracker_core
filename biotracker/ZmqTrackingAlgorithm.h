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
#include "zmq/ZmqClientProcess.h"

namespace BioTracker {
namespace Core {
namespace Zmq {

class ZmqTrackingAlgorithm : public TrackingAlgorithm {

  public:
    ZmqTrackingAlgorithm(std::shared_ptr<ZmqClientProcess> process, Settings &settings);

    virtual ~ZmqTrackingAlgorithm() override;

    void track(size_t frameNumber, const cv::Mat &frame) override;

    void paint(size_t frameNbr, ProxyMat &, const View &) override;

    void paintOverlay(size_t frameNbr, QPainter *, const View &) override;

    QPointer<QWidget> getToolsWidget() override;

    //    std::set<Qt::Key> &grabbedKeys() const override;

    void prepareSave() override;

    void postLoad() override;

    void postConnect() override;

  private:
    void mouseMoveEvent(QMouseEvent *) override;

    void mousePressEvent(QMouseEvent *) override;

    void mouseWheelEvent(QWheelEvent *) override;

    void keyPressEvent(QKeyEvent *) override;

    void shutdown();

  private Q_SLOTS:
    void btnClicked(const QString);
    void sldValueChanged(const QString, int value);

  private:
    std::set<Qt::Key> m_keys;
    std::shared_ptr<QWidget> m_tools;
    std::shared_ptr<ZmqClientProcess> m_process;
    EventHandler m_events;
};
}
}
}
