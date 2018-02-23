#pragma once

#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include "QAbstractButton"
#include "QPropertyAnimation"

class SwitchButton : public QAbstractButton {
    Q_OBJECT
    Q_PROPERTY(int offset READ offset WRITE setOffset)
    Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
    Q_PROPERTY(bool _switch READ state WRITE setState)

	public:
		SwitchButton(QString s0, QString s1, QWidget* parent = 0);
		//~SwitchButton();

        QSize sizeHint() const override;

        void setHeight(int h);
        int offset() const { return _x; }
        void setOffset(int o) {_x = o; update();}
        QBrush brush() const { return _brush;}
        void setBrush(const QBrush &brsh) { _brush = brsh;}
        bool state() const { return _switch;}
        void setState(bool toggle) { _switch = toggle;}

    signals:
        void emitSetEnabled(bool toggle);

    protected:
        void paintEvent(QPaintEvent* event) override;
        void mouseReleaseEvent(QMouseEvent*) override;

        bool _switch;
        qreal _opacity;
        int _x, _y, _height, _margin;
        QBrush _thumb, _track, _brush;
        QPropertyAnimation *_anim = nullptr;
        QString _s0;
        QString _s1;

};



#endif // SWITCHBUTTON_H