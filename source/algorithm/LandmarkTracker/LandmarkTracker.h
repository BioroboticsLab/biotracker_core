#ifndef LandmarkTracker_H
#define LandmarkTracker_H

#include <memory>
#include <string>
#include "glwidget.h"
#include "RectObject.h"
//#include "cv.h"
#include "source/tracking/TrackingAlgorithm.h"

#include <QLabel>
#include <QFormLayout>
#include <QPushButton>

class LandmarkTracker : public TrackingAlgorithm {
    Q_OBJECT

  private:

    std::shared_ptr<GLWidget> _glwidget;
    std::shared_ptr<QFrame> _roiFrame;

    size_t  _frameNr;
    size_t _rectId;

    cv::Rect _rectangle;

    cv::Point _selectorRecStart;
    cv::Point _selectorRecEnd;
    bool _showSelectorRec;
    bool _mouseMoved;
    bool _cubeIsRoi;

    cv::Point _mouseStart;
    cv::Point _mouseStop;

    //KML
    cv::Point pointOfText();
    cv::Point pot;

    std::string rectInfo();

    void drawRectangle(cv::Mat image);
    void setRectangle(cv::Mat image, cv::Scalar color);

    bool _lockRect;
    bool _movedRect;
    void lockRect();
    void unlockRect();

    cv::Rect box;
    cv::Mat selectedRoi;
    cv::Mat sampledRoi;

    boost::optional<cv::Mat> img;
    QPixmap _pixmap;

    QPixmap mat2QPixmap(const cv::Mat &mat);

    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

    virtual std::set<Qt::Key> const &grabbedKeys() const override;

    void forcePointIntoBorders(cv::Point &point, cv::Rect const &borders);






    std::shared_ptr<RectObject> _rectObject;

    //ParamsWidget GUI Elements

    QLabel *_roiInParams;
    QLabel *_sampledRoiInParams;

    QPushButton *_roiButton;
    QPushButton *_sampledRoiButton;

    void initParamsWidget();
    void updateParamsWidget();

    void addRect2TrackedObj();

    void setRect();

  public:

    cv::Scalar COLOR_RECTANGLE = cv::Scalar(0, 0, 255);

    LandmarkTracker(Settings &settings, QWidget *parent);
    ~LandmarkTracker(void) {}


    void track(ulong /* frameNumber */, const cv::Mat & /* frame */) override;
    void paint(ProxyPaintObject &, View const &view = OriginalView) override;
    void reset() override {}
    void postLoad() override;

    std::shared_ptr<QWidget> getToolsWidget() override {
        return std::static_pointer_cast<QWidget>(_glwidget);
    }
    std::shared_ptr<QWidget> getParamsWidget() override {
        return std::static_pointer_cast<QWidget>(_roiFrame);
    }

    //KML
    void defineROI(cv::Mat image);
    void startTool();

    cv::Mat getSelectedRoi();

    void samplingROI();

  public slots:

    void setLabelRoi();
    void setLabelSampledRoi();

};

#endif
