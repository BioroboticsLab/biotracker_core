#pragma once
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <QPainter>
#include <QWidget>
#include <stdlib.h>
#include <boost/python.hpp>
#include <numpy/ndarrayobject.h>
#include "../TrackingAlgorithm.h"
#include "../settings/Settings.h"

namespace BioTracker {
namespace Core {
namespace Interpreter {

class PyTrackingAlgorithm : public TrackingAlgorithm {
    Q_OBJECT
  public:

    PyTrackingAlgorithm(QString moduleName, Settings &s, QWidget *p);
    //~PyTrackingAlgorithm(void);

    void track(ulong frameNumber, const cv::Mat &frame) override;

    void paint(ProxyPaintObject &, const View &) override;

    void paintOverlay(QPainter *p) override;

    std::shared_ptr<QWidget> getToolsWidget() override;

    std::shared_ptr<QWidget> getParamsWidget() override;

//    std::set<Qt::Key> &grabbedKeys() const override;

    void prepareSave() override;

    void postLoad() override;

    void postConnect() override;

    /**
     * converts a c++ opencv matrix to a numpy matrix
     * @brief convert
     * @param mat
     * @return
     */
    static PyObject *convert(const cv::Mat &mat);

  private:
    void mouseMoveEvent(QMouseEvent *) override;

    void mousePressEvent(QMouseEvent *) override;

    void mouseWheelEvent(QWheelEvent *) override;

    void keyPressEvent(QKeyEvent *) override;

    bool isFunc(PyObject *pFunc);


  private:
    std::set<Qt::Key> m_keys;
    PyObject *m_pModule;
    PyObject *m_pTrackFunc;
};

}
}
}
