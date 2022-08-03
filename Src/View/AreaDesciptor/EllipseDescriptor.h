#pragma once

#include "AreaDescriptor.h"
#include <QBrush>
#include "util/types.h"

class EllipseDescriptor : public AreaDescriptor
{
    Q_OBJECT
public:
    EllipseDescriptor(IController* controller = 0, IModel* model = 0);
    ~EllipseDescriptor();
    void paint(QPainter*                       painter,
               const QStyleOptionGraphicsItem* option,
               QWidget*                        widget) override;

    QRectF boundingRect() const override;
    void   setBrush(QBrush brush) override;

    void                   setRect(std::vector<cv::Point> rect) override;
    std::vector<cv::Point> getRect() override;

    void updateRect() override;

    // IViewTrackedComponent interface
public Q_SLOTS:
    void getNotified();
    void receiveDragUpdate(BiotrackerTypes::AreaType vectorType,
                           int                       id,
                           double                    x,
                           double                    y);

protected:
    bool sceneEventFilter(QGraphicsItem* watched, QEvent* event) override;

private:
    void init();
    void updateEllipse();
    bool _isInit;

    std::vector<cv::Point> _v;
    // float _ellipseRotation;

    QGraphicsItem* _watchingDrag;
    QBrush         _brush;

    // Rects
    std::shared_ptr<QGraphicsEllipseItem> _rectification;
    std::shared_ptr<QGraphicsRectItem>    _rectificationMarkerOrig;
    std::shared_ptr<QGraphicsRectItem>    _rectificationMarkerEnd;

    int    _dragVectorId;
    QPoint _drag;

    // IView interface
public:
    void setNewModel(IModel* model) override
    {
        setModel(model);
    };

protected:
    void connectModelView() override{};
};
