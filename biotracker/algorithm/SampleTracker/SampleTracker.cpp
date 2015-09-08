#include "SampleTracker.h"

#include <QApplication>
#include <QIntValidator>
#include <QPushButton>
#include <QPainter>

#include "biotracker/core/Registry.h"

extern "C" {
    void registerTracker() {
        BioTracker::Core::Registry::getInstance().registerTrackerType<SampleTracker>("Sample Tracker");
    }
}

SampleTracker::SampleTracker(Settings &settings, QWidget *parent)
    : TrackingAlgorithm(settings, parent)
    , _paramsFrame(std::make_shared<QFrame>())
    , _toolsFrame(std::make_shared<QFrame>())
    , _showSelectorRec(false)
    , _showOriginal(false)
    , _lowH(0)
    , _highH(255)
    , _lowS(0)
    , _highS(255)
    , _lowV(0)
    , _highV(255) {
    initParamsFrame();
    initToolsFrame();
    _currentView="";
    _imageChanged=false;
}

void SampleTracker::track(ulong, const cv::Mat &imgOriginal) {
    //dont do nothing if we ain't got an image
    if (imgOriginal.empty()) {
        return;
    }
    using namespace cv;

    Mat imgHSV;

    cvtColor(imgOriginal, imgHSV,
             COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV


    inRange(imgHSV, Scalar(_lowH, _lowS, _lowV), Scalar(_highH, _highS, _highV),
            _imgTracked); //Threshold the image

    //morphological opening (remove small objects from the foreground)
    erode(_imgTracked, _imgTracked, getStructuringElement(MORPH_ELLIPSE, Size(5,
            5)));
    dilate(_imgTracked, _imgTracked, getStructuringElement(MORPH_ELLIPSE, Size(5,
            5)));

    //morphological closing (fill small holes in the foreground)
    cv::dilate(_imgTracked, _imgTracked,
               cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
    cv::erode(_imgTracked, _imgTracked, cv::getStructuringElement(cv::MORPH_ELLIPSE,
              cv::Size(5, 5)));
    _imageChanged = true;
}

/*
void SampleTracker::paint       (ProxyPaintObject& p , const View &view)
{
    if ( view.name != _currentView || _imageChanged)
    {
        _currentView = view.name;

        // getting image object will force videoView to upload it to graphic mem
        // this is necessarry even if we do not change the picture, since we changed
        // it before and otherwise we would still be stuck with the old one
        auto &image = p.getmat();
        if( !_imgTracked.empty() && _currentView == "Filter")
            {
                //since gui is expecting a BGR image we have to convert our grayscale image
                cv::cvtColor(_imgTracked, image, CV_GRAY2BGR);
            }
        _imageChanged=false;
    }


}
*/

void SampleTracker::paintOverlay(QPainter *painter) {
    if (_showSelectorRec) {
        drawRectangle(painter);
    }
}

//this will draw a basic rectangle
void SampleTracker::drawRectangle(QPainter *painter) {
    QColor color(Qt::cyan);
    color.setAlpha(150);
    QBrush brush(color);
    painter->fillRect(QRectF(_selectorRecStart, _selectorRecEnd),brush);

}

void SampleTracker::mouseMoveEvent(QMouseEvent *e) {
    if (_showSelectorRec) {
        _selectorRecEnd.setX(e->x());
        _selectorRecEnd.setY(e->y());
        //draw rectangle!
        Q_EMIT update();
    }
}
void SampleTracker::mousePressEvent(QMouseEvent *e) {
    //check if left button is clicked
    if (e->button() == Qt::LeftButton) {
        int x = e->x();
        int y = e->y();
        std::string note = "shift + left button press on: x="
                           + QString::number(x).toStdString() + " y=" + QString::number(y).toStdString();
        //initialize coordinates for selection tool
        _selectorRecStart.setX(e->x());
        _selectorRecStart.setY(e->y());
        _selectorRecEnd.setX(e->x());
        _selectorRecEnd.setY(e->y());
        _showSelectorRec = true;
    }
}
void SampleTracker::mouseReleaseEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        if (_showSelectorRec) {
            _selectorRecEnd.setX(e->x());
            _selectorRecEnd.setY(e->y());
            _showSelectorRec = false;
            //next draw will delete rectangle!
            Q_EMIT update();
            std::string note = "selected area from " + QString::number(
                                   _selectorRecStart.x()).toStdString() + ":"+
                               QString::number(_selectorRecStart.y()).toStdString()
                               + " to " +  QString::number(_selectorRecEnd.x()).toStdString() + ":"+
                               QString::number(_selectorRecEnd.y()).toStdString();
            Q_EMIT notifyGUI(note,MSGS::NOTIFICATION);
        }
    }
}

void SampleTracker::mouseWheelEvent(QWheelEvent *) {}

std::shared_ptr<QWidget> SampleTracker::getParamsWidget() {
    return _paramsFrame;
}

std::shared_ptr<QWidget> SampleTracker::getToolsWidget() {
    return _toolsFrame;
}

void SampleTracker::initToolsFrame() {
    QFormLayout *layout = new QFormLayout(_toolsFrame.get());
    layout->addRow(_colorBut);
    _toolsFrame->setLayout(layout);
}


void SampleTracker::initParamsFrame() {
    _lowHEdit  = new QLineEdit(_paramsFrame.get());
    _highHEdit = new QLineEdit(_paramsFrame.get());
    _lowSEdit  = new QLineEdit(_paramsFrame.get());
    _highSEdit = new QLineEdit(_paramsFrame.get());
    _lowVEdit  = new QLineEdit(_paramsFrame.get());
    _highVEdit = new QLineEdit(_paramsFrame.get());

    _colorBut = new QPushButton("change color!", _paramsFrame.get());

    _lowHEdit->setValidator(new QIntValidator(0, 255, _paramsFrame.get()));
    _highHEdit->setValidator(new QIntValidator(0, 255, _paramsFrame.get()));
    _lowSEdit->setValidator(new QIntValidator(0, 255, _paramsFrame.get()));
    _highSEdit->setValidator(new QIntValidator(0, 255, _paramsFrame.get()));
    _lowVEdit->setValidator(new QIntValidator(0, 255, _paramsFrame.get()));
    _highVEdit->setValidator(new QIntValidator(0, 255, _paramsFrame.get()));

    _lowHEdit->setText(QString::number(_lowH));
    _highHEdit->setText(QString::number(_highH));
    _lowSEdit->setText(QString::number(_lowS));
    _highSEdit->setText(QString::number(_highS));
    _lowVEdit->setText(QString::number(_lowV));
    _highVEdit->setText(QString::number(_highV));

    QFormLayout *layout = new QFormLayout(_paramsFrame.get());
    layout->addRow("H-low",  _lowHEdit);
    layout->addRow("H-high", _highHEdit);
    layout->addRow("S-low",  _lowSEdit);
    layout->addRow("S-high", _highSEdit);
    layout->addRow("V-low",  _lowVEdit);
    layout->addRow("V-high", _highVEdit);
    _paramsFrame->setLayout(layout);

    QObject::connect(this->_colorBut, SIGNAL(clicked()), this,
                     SLOT(changeFilterColor()));
}

void SampleTracker::changeFilterColor() {
    _lowH  =    _lowHEdit->text().toInt();
    _highH =    _highHEdit->text().toInt();
    _lowS  =    _lowSEdit->text().toInt();
    _highS =    _highSEdit->text().toInt();
    _lowV  =    _lowVEdit->text().toInt();
    _highV =    _highVEdit->text().toInt();
    Q_EMIT forceTracking();
}

void SampleTracker::postConnect() {
    Q_EMIT registerViews({ { "Filter" } });
}
