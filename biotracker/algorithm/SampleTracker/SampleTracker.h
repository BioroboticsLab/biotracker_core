#pragma once

#include "biotracker/core/TrackingAlgorithm.h"

#include <QGroupBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

class SampleTracker : public TrackingAlgorithm {
    Q_OBJECT
  public:
    SampleTracker(Settings &settings, QWidget *parent);

    void track(ulong frameNumber, const cv::Mat &frame) override;
    /*
    void paint (ProxyPaintObject&,View const& view = OriginalView) override;
    */
    void paintOverlay(QPainter *painter) override;
    void postConnect() override;

    std::shared_ptr<QWidget> getParamsWidget() override;
    std::shared_ptr<QWidget> getToolsWidget() override;

  public slots:
    void changeFilterColor();

  private:
    std::shared_ptr<QFrame> _paramsFrame;
    std::shared_ptr<QFrame> _toolsFrame;
    QPointF _selectorRecStart;
    QPointF _selectorRecEnd;
    void drawRectangle(QPainter *painter);
    bool _showSelectorRec;
    bool _showOriginal;
    std::vector<TrackedObject> _trackedObjects;
    void initParamsFrame();
    void initToolsFrame();
    //values for filtering a color (HSV format)
    int _lowH;
    int _highH;
    int _lowS;
    int _highS;
    int _lowV;
    int _highV;
    cv::Mat _imgTracked;
    //gui elements to set those values
    QLineEdit *_lowHEdit;
    QLineEdit *_highHEdit;
    QLineEdit *_lowSEdit;
    QLineEdit *_highSEdit;
    QLineEdit *_lowVEdit;
    QLineEdit *_highVEdit;

    QPushButton *_colorBut;
    std::string _currentView;
    bool _imageChanged;
    //mouse click and move events
    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseWheelEvent(QWheelEvent *e) override;
};
