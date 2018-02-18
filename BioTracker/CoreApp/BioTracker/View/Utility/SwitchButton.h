#pragma once

#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include "QAbstractButton"
#include "QPropertyAnimation"

class SwitchButton : public QAbstractButton {
    Q_OBJECT
    Q_PROPERTY(int offset READ offset WRITE setOffset)
    Q_PROPERTY(QBrush brush READ brush WRITE setBrush)


	public:
		SwitchButton(QWidget* parent = 0);
		//~SwitchButton();

        QSize sizeHint() const override;

        void setHeight(int h);
        int offset() const { return _x; }
        void setOffset(int o) {_x = o; update();}
        QBrush brush() const { return _brush;}
        void setBrush(const QBrush &brsh) { _brush = brsh;}

    signals:
        void emitSetTracking(bool toggle);

    protected:
        void paintEvent(QPaintEvent* event) override;
        void mouseReleaseEvent(QMouseEvent*) override;

        bool _switch;
        qreal _opacity;
        int _x, _y, _height, _margin;
        QBrush _thumb, _track, _brush;
        QPropertyAnimation *_anim = nullptr;

};



#endif // SWITCHBUTTON_H