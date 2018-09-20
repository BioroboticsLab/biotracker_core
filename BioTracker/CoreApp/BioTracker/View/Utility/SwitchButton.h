#pragma once

#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include "QAbstractButton"
#include "QPropertyAnimation"

/**
* This class inherits QAbstractButton.
* It is an utility class to create android-like animated switches
* inspiration: https://stackoverflow.com/questions/14780517/toggle-switch-in-qt
*/
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
	void setOffset(int o) { _x = o; update(); }
	QBrush brush() const { return _brush; }
	void setBrush(const QBrush &brsh) { _brush = brsh; }
	bool state() const { return _switch; }
	void setState(bool toggle) { _switch = toggle; animateSwitch(); update(); }

	void animateSwitch();

signals:
	void emitSetEnabled(bool toggle);

protected:
	void paintEvent(QPaintEvent* event) override;
	void mouseReleaseEvent(QMouseEvent*) override;
	void enterEvent(QEvent *e) override;


	bool _switch;						/**< state fo the switch  */
	qreal _opacity;                     /**< opacity  */
	int _x, _y, _height, _margin;       /**< position and dimensions  */
	QBrush _thumb, _track, _brush;
	QPropertyAnimation *_anim = nullptr;/**< animation  */
	QString _s0;                        /**< text in state 0  */
	QString _s1;                        /**< text in state 1  */

};



#endif // SWITCHBUTTON_H