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

    /**
     * @brief listenToEvents
     * This function MUST be called whenever the zmq tracker calls its client as
     * we need to wait for user input to "stay responsive"
     * CAREFUL! This function is NOT threadsafe and must be called from
     * a protected environment (protected by m_zmqMutex)
     */
    void listenToEvents();

  private Q_SLOTS:
    void btnClicked();
    void sldValueChanged(int value);

  private:
    std::unique_ptr<QProcess> m_zmqClient;
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
